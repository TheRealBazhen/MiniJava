#include <program/visitor.h>

Variable::Variable(const std::string& name) : name(name) {
}

void Variable::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

ArrayElement::ArrayElement(
    const std::string& name,
    std::shared_ptr<Expression> index
) : name(name), index(index) {
}

void ArrayElement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}
