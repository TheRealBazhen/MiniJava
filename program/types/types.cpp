#include <program/visitor.h>

SimpleType::SimpleType(const std::string& name) : name(name) {
}

void SimpleType::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

std::string SimpleType::GetTypeName() {
    return name;
}

ArrayType::ArrayType(std::shared_ptr<SimpleType> element_type) : element_type(element_type) {
}

void ArrayType::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

std::string ArrayType::GetTypeName() {
    return element_type->name + "[]";
}
