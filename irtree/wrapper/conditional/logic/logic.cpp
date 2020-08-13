#include <irtree/wrapper/conditional/logic/logic.h>

#include <irtree/nodes/statement/label.h>
#include <irtree/nodes/statement/sequence.h>

namespace IR {
AndConditionalWrapper::AndConditionalWrapper(
    std::shared_ptr<SubtreeWrapper> lhs,
    std::shared_ptr<SubtreeWrapper> rhs
) : lhs_(lhs), rhs_(rhs) {
}

std::shared_ptr<Statement> AndConditionalWrapper::ToConditional(const Label& true_label, const Label& false_label) {
    Label middle_label;

    return std::make_shared<SequenceStatement>(
        lhs_->ToConditional(middle_label, false_label),
        std::make_shared<SequenceStatement>(
            std::make_shared<LabelStatement>(middle_label),
            rhs_->ToConditional(true_label, false_label)
        )
    );
}

OrConditionalWrapper::OrConditionalWrapper(std::shared_ptr<SubtreeWrapper> lhs, std::shared_ptr<SubtreeWrapper> rhs)
    : lhs_(lhs), rhs_(rhs) {
}

std::shared_ptr<Statement> OrConditionalWrapper::ToConditional(const Label& true_label, const Label& false_label) {
    Label middle_label;

    return std::make_shared<SequenceStatement>(
        lhs_->ToConditional(true_label, middle_label),
        std::make_shared<SequenceStatement>(
            std::make_shared<LabelStatement>(middle_label),
            rhs_->ToConditional(true_label, false_label)
        )
    );
}

NegateConditionalWrapper::NegateConditionalWrapper(std::shared_ptr<SubtreeWrapper> expr) : expr_(expr) {
}

std::shared_ptr<Statement> NegateConditionalWrapper::ToConditional(const Label& true_label, const Label& false_label) {
    return expr_->ToConditional(false_label, true_label);
}
}
