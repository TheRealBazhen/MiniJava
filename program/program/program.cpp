#include <program/visitor.h>

Program::Program(
    std::shared_ptr<MainClassDecl> main_class,
    std::shared_ptr<ClassDeclList> other_classes
) : main_class(main_class), other_classes(other_classes) {
}

void Program::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}
