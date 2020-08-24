#include <irtree/visitors/eseq_mover/eseq_mover.h>

#include <irtree/wrapper/expression/expression.h>
#include <irtree/wrapper/statement/statement.h>

namespace IR {
struct ESEQData{
    std::shared_ptr<Statement> side_effect;
    std::vector<std::shared_ptr<Expression>> expressions;
};

ESEQData SimplifyExpressionList(const std::vector<std::shared_ptr<Expression>>& expressions) {
    ESEQData result;
    size_t start_index = 0;
    size_t size = expressions.size();
    result.expressions.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        auto expr_eseq = std::dynamic_pointer_cast<ESEQExpression>(expressions[i]);
        if (expr_eseq) {
            for (size_t j = start_index; j < i; ++j) {
                if (!std::dynamic_pointer_cast<ConstantExpression>(result.expressions[j]) &&
                    !std::dynamic_pointer_cast<NameExpression>(result.expressions[j])) {
                    Temporary temp;
                    auto side_effect = std::make_shared<MoveStatement>(
                        std::make_shared<TemporaryExpression>(temp),
                        result.expressions[j]
                    );
                    result.expressions[j] = std::make_shared<TemporaryExpression>(temp);
                    if (result.side_effect) {
                        result.side_effect = std::make_shared<SequenceStatement>(result.side_effect, side_effect);
                    } else {
                        result.side_effect = side_effect;
                    }
                }
            }
            start_index = i;
            if (result.side_effect) {
                result.side_effect = std::make_shared<SequenceStatement>(result.side_effect, expr_eseq->side_effect);
            } else {
                result.side_effect = expr_eseq->side_effect;
            }
            result.expressions.push_back(expr_eseq->value);
        } else {
            result.expressions.push_back(expressions[i]);
        }
    }
    return result;
}

std::shared_ptr<IR::SubtreeWrapper> ESEQMover::Visit(std::shared_ptr<Node> node) {
    node->Accept(shared_from_this());
    return node_;
}

std::shared_ptr<Statement> ESEQMover::GetTree() {
    return node_->ToStatement();
}

void ESEQMover::Visit(std::shared_ptr<BinaryOperationExpression> expr) {
    auto lhs = Visit(expr->lhs)->ToExpression();
    auto rhs = Visit(expr->rhs)->ToExpression();
    auto data = SimplifyExpressionList({lhs, rhs});
    if (data.side_effect) {
        node_ = std::make_shared<ExpressionWrapper>(
            std::make_shared<ESEQExpression>(
                data.side_effect,
                std::make_shared<BinaryOperationExpression>(
                    expr->type, data.expressions[0], data.expressions[1]
                )
            )
        );
    } else {
        node_ = std::make_shared<ExpressionWrapper>(
            std::make_shared<BinaryOperationExpression>(expr->type, data.expressions[0], data.expressions[1])
        );
    }
}

void ESEQMover::Visit(std::shared_ptr<CallExpression> expr) {
    std::vector<std::shared_ptr<Expression>> expressions;
    expressions.reserve(expr->arguments->expressions.size() + 1);
    expressions.push_back(Visit(expr->function)->ToExpression());
    for (auto arg : expr->arguments->expressions) {
        expressions.push_back(Visit(arg)->ToExpression());
    }
    auto data = SimplifyExpressionList(expressions);
    auto arguments = std::make_shared<ExpressionList>();
    for (size_t i = 1; i < expressions.size(); ++i) {
        arguments->AddExpression(data.expressions[i]);
    }
    if (data.side_effect) {
        node_ = std::make_shared<ExpressionWrapper>(
            std::make_shared<ESEQExpression>(
                data.side_effect,
                std::make_shared<CallExpression>(data.expressions[0], arguments)
            )
        );
    } else {
        node_ = std::make_shared<ExpressionWrapper>(std::make_shared<CallExpression>(data.expressions[0], arguments));
    }
}

void ESEQMover::Visit(std::shared_ptr<ConstantExpression> expr) {
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<ConstantExpression>(expr->value));
}

void ESEQMover::Visit(std::shared_ptr<ESEQExpression> expr) {
    auto side_effect = Visit(expr->side_effect)->ToStatement();
    auto value = Visit(expr->value)->ToExpression();
    auto value_eseq = std::dynamic_pointer_cast<ESEQExpression>(value);
    if (value_eseq) {
        node_ = std::make_shared<ExpressionWrapper>(
            std::make_shared<ESEQExpression>(
                std::make_shared<SequenceStatement>(side_effect, value_eseq->side_effect),
                value_eseq->value
            )
        );
    } else {
        node_ = std::make_shared<ExpressionWrapper>(std::make_shared<ESEQExpression>(side_effect, value));
    }
}

void ESEQMover::Visit(std::shared_ptr<MemoryAccessExpression> expr) {
    auto address = Visit(expr->address)->ToExpression();
    auto address_eseq = std::dynamic_pointer_cast<ESEQExpression>(address);
    if (address_eseq) {
        node_ = std::make_shared<ExpressionWrapper>(
            std::make_shared<ESEQExpression>(
                address_eseq->side_effect,
                std::make_shared<MemoryAccessExpression>(address_eseq->value)
            )
        );
    } else {
        node_ = std::make_shared<ExpressionWrapper>(std::make_shared<MemoryAccessExpression>(address));
    }
}

void ESEQMover::Visit(std::shared_ptr<NameExpression> expr) {
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<NameExpression>(expr->label));
}

void ESEQMover::Visit(std::shared_ptr<TemporaryExpression> expr) {
    node_ = std::make_shared<ExpressionWrapper>(std::make_shared<TemporaryExpression>(expr->temp));
}

void ESEQMover::Visit(std::shared_ptr<ConditionalJumpStatement> stmt) {
    auto lhs = Visit(stmt->lhs)->ToExpression();
    auto rhs = Visit(stmt->rhs)->ToExpression();
    auto data = SimplifyExpressionList({lhs, rhs});
    if (data.side_effect) {
        node_ = std::make_shared<StatementWrapper>(
            std::make_shared<SequenceStatement>(
                data.side_effect,
                std::make_shared<ConditionalJumpStatement>(
                    stmt->cond_operator,
                    data.expressions[0], data.expressions[1],
                    stmt->true_label, stmt->false_label
                )
            )
        );
    } else {
        node_ = std::make_shared<StatementWrapper>(
            std::make_shared<ConditionalJumpStatement>(
                stmt->cond_operator,
                data.expressions[0], data.expressions[1],
                stmt->true_label, stmt->false_label
            )
        );
    }
}

void ESEQMover::Visit(std::shared_ptr<ExpressionStatement> stmt) {
    auto expr = Visit(stmt->expression)->ToExpression();
    auto expr_eseq = std::dynamic_pointer_cast<ESEQExpression>(expr);
    if (expr_eseq) {
        node_ = std::make_shared<StatementWrapper>(
            std::make_shared<SequenceStatement>(
                expr_eseq->side_effect,
                std::make_shared<ExpressionStatement>(expr_eseq->value)
            )
        );
    } else {
        node_ = std::make_shared<StatementWrapper>(std::make_shared<ExpressionStatement>(expr));
    }
}

void ESEQMover::Visit(std::shared_ptr<JumpStatement> stmt) {
    auto address = Visit(stmt->address)->ToExpression();
    auto address_eseq = std::dynamic_pointer_cast<ESEQExpression>(address);
    if (address_eseq) {
        node_ = std::make_shared<StatementWrapper>(
            std::make_shared<SequenceStatement>(
                address_eseq->side_effect,
                std::make_shared<JumpStatement>(address_eseq->value)
            )
        );
    } else {
        node_ = std::make_shared<StatementWrapper>(std::make_shared<JumpStatement>(address));
    }
}

void ESEQMover::Visit(std::shared_ptr<LabelStatement> stmt) {
    node_ = std::make_shared<StatementWrapper>(std::make_shared<LabelStatement>(stmt->label));
}

void ESEQMover::Visit(std::shared_ptr<MoveStatement> stmt) {
    auto dest = Visit(stmt->destination)->ToExpression();
    auto src = Visit(stmt->source)->ToExpression();

    auto dest_memory_access = std::dynamic_pointer_cast<MemoryAccessExpression>(dest);

    std::shared_ptr<Statement> side_effect;

    if (dest_memory_access) {
        auto data = SimplifyExpressionList({dest_memory_access->address, src});
        side_effect = data.side_effect;
        dest = std::make_shared<MemoryAccessExpression>(data.expressions[0]);
        src = data.expressions[1];
    } else {
        auto data = SimplifyExpressionList({src});
        side_effect = data.side_effect;
        src = data.expressions[0];
    }

    std::shared_ptr<Statement> res_stmt = std::make_shared<MoveStatement>(dest, src);
    if (side_effect) {
        res_stmt = std::make_shared<SequenceStatement>(side_effect, res_stmt);
    }
    node_ = std::make_shared<StatementWrapper>(res_stmt);
}

void ESEQMover::Visit(std::shared_ptr<SequenceStatement> stmt) {
    auto first = Visit(stmt->first)->ToStatement();
    auto second = Visit(stmt->second)->ToStatement();
    node_ = std::make_shared<StatementWrapper>(std::make_shared<SequenceStatement>(first, second));
}

void ESEQMover::Visit(std::shared_ptr<ExpressionList> list) {
    // Ignore
}

}