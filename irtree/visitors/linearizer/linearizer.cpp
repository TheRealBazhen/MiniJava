#include <irtree/visitors/linearizer/linearizer.h>

#include <irtree/wrapper/expression/expression.h>
#include <irtree/wrapper/statement/statement.h>

#include <stdexcept>

namespace IR {
std::shared_ptr<IR::SubtreeWrapper> Linearizer::Visit(std::shared_ptr<Node> node) {
    node->Accept(shared_from_this());
    return node_;
}

std::shared_ptr<Statement> Linearizer::GetTree() {
    return node_->ToStatement();
}

void Linearizer::Visit(std::shared_ptr<BinaryOperationExpression> expr) {
    auto lhs = Visit(expr->lhs)->ToExpression();
    auto rhs = Visit(expr->rhs)->ToExpression();
    node_ = std::make_shared<ExpressionWrapper>(
        std::make_shared<BinaryOperationExpression>(expr->type, lhs, rhs)
    );
}

void Linearizer::Visit(std::shared_ptr<CallExpression> expr) {
    auto function = Visit(expr->function)->ToExpression();
    auto arguments = std::make_shared<ExpressionList>();
    for (auto arg : expr->arguments->expressions) {
        arguments->AddExpression(Visit(arg)->ToExpression());
    }
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<CallExpression>(function, arguments));
}

void Linearizer::Visit(std::shared_ptr<ConstantExpression> expr) {
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<ConstantExpression>(expr->value));
}

void Linearizer::Visit(std::shared_ptr<ESEQExpression> expr) {
    /// Should not be here
    throw std::runtime_error("Use ESEQ mover before linearizer");
}

void Linearizer::Visit(std::shared_ptr<MemoryAccessExpression> expr) {
    auto address = Visit(expr->address)->ToExpression();
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<MemoryAccessExpression>(address));
}

void Linearizer::Visit(std::shared_ptr<NameExpression> expr) {
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<NameExpression>(expr->label));
}

void Linearizer::Visit(std::shared_ptr<TemporaryExpression> expr) {
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<TemporaryExpression>(expr->temp));
}

void Linearizer::Visit(std::shared_ptr<ConditionalJumpStatement> stmt) {
    auto lhs = Visit(stmt->lhs)->ToExpression();
    auto rhs = Visit(stmt->rhs)->ToExpression();

    node_ = std::make_shared<StatementWrapper>(
        std::make_shared<ConditionalJumpStatement>(
            stmt->cond_operator,
            lhs, rhs,
            stmt->true_label, stmt->false_label
        )
    );
}

void Linearizer::Visit(std::shared_ptr<ExpressionStatement> stmt) {
    node_ = Visit(stmt->expression);
}

void Linearizer::Visit(std::shared_ptr<JumpStatement> stmt) {
    auto address = Visit(stmt->address)->ToExpression();
    node_ = std::make_shared<StatementWrapper>(std::make_shared<JumpStatement>(address));
}

void Linearizer::Visit(std::shared_ptr<LabelStatement> stmt) {
    node_ = std::make_shared<StatementWrapper>(std::make_shared<LabelStatement>(stmt->label));
}

void Linearizer::Visit(std::shared_ptr<MoveStatement> stmt) {
    auto dest = Visit(stmt->destination)->ToExpression();
    auto src = Visit(stmt->source)->ToExpression();

    node_ = std::make_shared<StatementWrapper>(std::make_shared<MoveStatement>(dest, src));
}

std::shared_ptr<SequenceStatement> GetLastSeqInChain(std::shared_ptr<SequenceStatement> stmt) {
    while (true) {
        auto seq_second = std::dynamic_pointer_cast<SequenceStatement>(stmt->second);
        if (!seq_second) {
            break;
        }
        stmt = seq_second;
    }
    return stmt;
}

void Linearizer::Visit(std::shared_ptr<SequenceStatement> stmt) {
    auto first = Visit(stmt->first)->ToStatement();
    auto second = Visit(stmt->second)->ToStatement();

    auto first_seq = std::dynamic_pointer_cast<SequenceStatement>(first);
    if (first_seq) {
        auto last_seq = GetLastSeqInChain(first_seq);
        last_seq->second = std::make_shared<SequenceStatement>(last_seq->second, second);
        node_ = std::make_shared<StatementWrapper>(first_seq);
    } else {
        node_ = std::make_shared<StatementWrapper>(std::make_shared<SequenceStatement>(first, second));
    }
}

void Linearizer::Visit(std::shared_ptr<ExpressionList> list) {
    // Ignore
}
}
