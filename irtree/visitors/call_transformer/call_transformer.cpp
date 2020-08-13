#include <irtree/visitors/call_transformer/call_transformer.h>

#include <irtree/wrapper/expression/expression.h>
#include <irtree/wrapper/statement/statement.h>

namespace IR {
std::shared_ptr<Statement> CallTransformer::GetTree() {
    return node_->ToStatement();
}

std::shared_ptr<IR::SubtreeWrapper> CallTransformer::Visit(std::shared_ptr<Node> node, bool moving_to_register) {
    is_moving_to_register_ = moving_to_register;
    node->Accept(shared_from_this());
    return node_;
}

void CallTransformer::Visit(std::shared_ptr<BinaryOperationExpression> expr) {
    auto lhs = Visit(expr->lhs)->ToExpression();
    auto rhs = Visit(expr->rhs)->ToExpression();
    node_ = std::make_shared<ExpressionWrapper>(
        std::make_shared<BinaryOperationExpression>(expr->type, lhs, rhs)
    );
}

void CallTransformer::Visit(std::shared_ptr<CallExpression> expr) {
    bool need_transform = !is_moving_to_register_;
    auto function = Visit(expr->function)->ToExpression();
    auto arguments = std::make_shared<ExpressionList>();
    for (auto arg : expr->arguments->expressions) {
        arguments->AddExpression(Visit(arg)->ToExpression());
    }
    if (need_transform) {
        Temporary temp;
        node_ = std::make_shared<ExpressionWrapper>(
            std::make_shared<ESEQExpression>(
                std::make_shared<MoveStatement>(
                    std::make_shared<TemporaryExpression>(temp),
                    std::make_shared<CallExpression>(function, arguments)
                ),
                std::make_shared<TemporaryExpression>(temp)
            )
        );
    } else {
        node_ = std::make_shared<ExpressionWrapper>(std::make_shared<CallExpression>(function, arguments));
    }
}

void CallTransformer::Visit(std::shared_ptr<ConstantExpression> expr) {
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<ConstantExpression>(expr->value));
}

void CallTransformer::Visit(std::shared_ptr<ESEQExpression> expr) {
    auto side_effect = Visit(expr->side_effect)->ToStatement();
    auto value = Visit(expr->value)->ToExpression();
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<ESEQExpression>(side_effect, value));
}

void CallTransformer::Visit(std::shared_ptr<MemoryAccessExpression> expr) {
    auto address = Visit(expr->address)->ToExpression();
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<MemoryAccessExpression>(address));
}

void CallTransformer::Visit(std::shared_ptr<NameExpression> expr) {
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<NameExpression>(expr->label));
}

void CallTransformer::Visit(std::shared_ptr<TemporaryExpression> expr) {
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<TemporaryExpression>(expr->temp));
}

void CallTransformer::Visit(std::shared_ptr<ConditionalJumpStatement> stmt) {
    auto lhs = Visit(stmt->lhs)->ToExpression();
    auto rhs = Visit(stmt->rhs)->ToExpression();
    node_ = std::make_shared<StatementWrapper>(
        std::make_shared<ConditionalJumpStatement>(stmt->cond_operator, lhs, rhs, stmt->true_label, stmt->false_label)
    );
}

void CallTransformer::Visit(std::shared_ptr<ExpressionStatement> stmt) {
    node_ = Visit(stmt->expression, true);
}

void CallTransformer::Visit(std::shared_ptr<JumpStatement> stmt) {
    auto address = Visit(stmt->address)->ToExpression();
    node_ = std::make_shared<StatementWrapper>(std::make_shared<JumpStatement>(address));
}

void CallTransformer::Visit(std::shared_ptr<LabelStatement> stmt) {
    node_ = std::make_shared<StatementWrapper>(std::make_shared<LabelStatement>(stmt->label));
}

void CallTransformer::Visit(std::shared_ptr<MoveStatement> stmt) {
    auto dest = Visit(stmt->destination)->ToExpression();
    bool move_to_register = false;
    if (std::dynamic_pointer_cast<TemporaryExpression>(dest)) {
        move_to_register = true;
    }
    auto src = Visit(stmt->source, move_to_register)->ToExpression();
    node_ = std::make_shared<StatementWrapper>(std::make_shared<MoveStatement>(dest, src));
}

void CallTransformer::Visit(std::shared_ptr<SequenceStatement> stmt) {
    auto first = Visit(stmt->first)->ToStatement();
    auto second = Visit(stmt->second)->ToStatement();
    node_ = std::make_shared<StatementWrapper>(std::make_shared<SequenceStatement>(first, second));
}

void CallTransformer::Visit(std::shared_ptr<ExpressionList> list) {
    // Ignore
}
}
