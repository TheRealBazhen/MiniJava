#include <program/visitor.h>

ArgumentDecl::ArgumentDecl(std::shared_ptr<GrammaticsType> type, const std::string& name)
    : type(type), name(name) {
}

void ArgumentDecl::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

void ArgumentDeclList::AddArgumentDecl(std::shared_ptr<ArgumentDecl> argument) {
    arguments.push_back(argument);
}

void ArgumentDeclList::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

void ArgumentValues::AddValue(std::shared_ptr<Expression> value) {
    values.push_back(value);
}

void ArgumentValues::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}
