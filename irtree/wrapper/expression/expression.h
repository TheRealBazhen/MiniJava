#ifndef WRAPPER_EXPRESSION_H
#define WRAPPER_EXPRESSION_H

#include <irtree/wrapper/wrapper.h>

namespace IR {
class ExpressionWrapper : public SubtreeWrapper {
public:
    ExpressionWrapper(std::shared_ptr<Expression> expression);

    std::shared_ptr<Expression> ToExpression() override;
    std::shared_ptr<Statement> ToStatement() override;
    std::shared_ptr<Statement> ToConditional(const Label& true_label, const Label& false_label) override;

private:
    std::shared_ptr<Expression> expression_;
};
}

#endif
