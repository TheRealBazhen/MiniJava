#ifndef PROGRAM_H
#define PROGRAM_H

#include <program/declaration/class.h>

class Program : public Node, public std::enable_shared_from_this<Program> {
public:
    Program(std::shared_ptr<MainClassDecl> main_class, std::shared_ptr<ClassDeclList> other_classes);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<MainClassDecl> main_class;
    std::shared_ptr<ClassDeclList> other_classes;
};

#endif