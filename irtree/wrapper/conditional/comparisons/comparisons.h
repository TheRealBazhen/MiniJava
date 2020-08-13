#ifndef WRAPPER_COMPARISONS_H
#define WRAPPER_COMPARISONS_H

#include <irtree/nodes/statement/conditional_jump.h>
#include <irtree/wrapper/conditional/conditional.h>

namespace IR {
class ComparisonWrapper : public ConditionalWrapper {
public:
    ComparisonWrapper(
        ConditionalOperatorType operator_type,
        std::shared_ptr<Expression> lhs,
        std::shared_ptr<Expression> rhs
    );

    std::shared_ptr<Statement> ToConditional(const Label& true_label, const Label& false_label) override;

private:
    ConditionalOperatorType operator_type_;
    std::shared_ptr<Expression> lhs_;
    std::shared_ptr<Expression> rhs_;
};
}

#endif
