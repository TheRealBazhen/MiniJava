#ifndef WRAPPER_LOGIC_H
#define WRAPPER_LOGIC_H

#include <irtree/wrapper/conditional/conditional.h>

namespace IR {
class AndConditionalWrapper : public ConditionalWrapper {
public:
    AndConditionalWrapper(std::shared_ptr<SubtreeWrapper> lhs, std::shared_ptr<SubtreeWrapper> rhs);

    std::shared_ptr<Statement> ToConditional(const Label& true_label, const Label& false_label) override;

private:
    std::shared_ptr<SubtreeWrapper> lhs_;
    std::shared_ptr<SubtreeWrapper> rhs_;
};

class OrConditionalWrapper : public ConditionalWrapper {
public:
    OrConditionalWrapper(std::shared_ptr<SubtreeWrapper> lhs, std::shared_ptr<SubtreeWrapper> rhs);

    std::shared_ptr<Statement> ToConditional(const Label& true_label, const Label& false_label) override;

private:
    std::shared_ptr<SubtreeWrapper> lhs_;
    std::shared_ptr<SubtreeWrapper> rhs_;
};

class NegateConditionalWrapper : public ConditionalWrapper {
public:
    NegateConditionalWrapper(std::shared_ptr<SubtreeWrapper> expr);

    std::shared_ptr<Statement> ToConditional(const Label& true_label, const Label& false_label) override;

private:
    std::shared_ptr<SubtreeWrapper> expr_;
};

}

#endif
