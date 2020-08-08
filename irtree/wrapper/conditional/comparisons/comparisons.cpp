#include <irtree/wrapper/conditional/comparisons/comparisons.h>

namespace IR {
ComparisonWrapper::ComparisonWrapper(
    ConditionalOperatorType operator_type,
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : operator_type_(operator_type), lhs_(lhs), rhs_(rhs) {
}

std::shared_ptr<Statement> ComparisonWrapper::ToConditional(const Label& true_label, const Label& false_label) {
    return std::make_shared<ConditionalJumpStatement>(
        operator_type_,
        lhs_,
        rhs_,
        true_label,
        false_label
    );
}
}
