#include <irtree/visitors/visitor.h>

namespace IR {
ConditionalJumpStatement::ConditionalJumpStatement(
    ConditionalOperatorType cond_operator,
    std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs,
    const Label& true_label, const Label& false_label
) : cond_operator(cond_operator), lhs(lhs), rhs(rhs), true_label(true_label), false_label(false_label) {
}

void ConditionalJumpStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

ExpressionStatement::ExpressionStatement(std::shared_ptr<Expression> expression) : expression(expression) {
}

void ExpressionStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

JumpStatement::JumpStatement(std::shared_ptr<Expression> address) : address(address) {
}

void JumpStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

LabelStatement::LabelStatement(const Label& label) : label(label) {
}

void LabelStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

MoveStatement::MoveStatement(std::shared_ptr<Expression> destination, std::shared_ptr<Expression> source)
    : destination(destination), source(source) {
}

void MoveStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

SequenceStatement::SequenceStatement(std::shared_ptr<Statement> first, std::shared_ptr<Statement> second)
    : first(first), second(second) {
}

void SequenceStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}
}
