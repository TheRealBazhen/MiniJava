#include <irtree/wrapper/expression/expression.h>

#include <irtree/nodes/expression/constant.h>
#include <irtree/nodes/statement/conditional_jump.h>
#include <irtree/nodes/statement/expression.h>

namespace IR {
ExpressionWrapper::ExpressionWrapper(std::shared_ptr<Expression> expression)
    : expression_(expression) {
}

std::shared_ptr<Expression> ExpressionWrapper::ToExpression() {
    return expression_;
}

std::shared_ptr<Statement> ExpressionWrapper::ToStatement() {
    return std::make_shared<ExpressionStatement>(expression_);
}

std::shared_ptr<Statement> ExpressionWrapper::ToConditional(const Label& true_label, const Label& false_label) {
    return std::make_shared<ConditionalJumpStatement>(
        ConditionalOperatorType::NE,
        expression_,
        std::make_shared<ConstantExpression>(0),
        true_label,
        false_label
    );
}
}
