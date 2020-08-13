#include <program/visitor.h>

ClassDeclaration::ClassDeclaration(
    const std::string& name,
    const std::string& parent,
    std::shared_ptr<DeclarationList> declarations
): name(name), parent(parent), declarations(declarations) {
}

ClassDeclaration::ClassDeclaration(
    const std::string& name,
    std::shared_ptr<DeclarationList> declarations
) : name(name), declarations(declarations) {
}

void ClassDeclaration::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

void ClassDeclList::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

void ClassDeclList::AddDeclaration(std::shared_ptr<ClassDeclaration> declaration) {
    declarations.push_back(declaration);
}

MainClassDecl::MainClassDecl(
    const std::string& name,
    std::shared_ptr<StatementList> main_statements
) : name(name), main_statements(main_statements) {
}

void MainClassDecl::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

void DeclarationList::AddDeclaration(std::shared_ptr<Declaration> declaration) {
    declarations.push_back(declaration);
}

void DeclarationList::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

MethodDecl::MethodDecl(
    std::shared_ptr<GrammaticsType> type,
    const std::string& name,
    std::shared_ptr<ArgumentDeclList> args,
    std::shared_ptr<StatementList> statements
) : type(type), name(name), arguments(args), statements(statements) {
}

MethodDecl::MethodDecl(
    std::shared_ptr<GrammaticsType> type,
    const std::string& name,
    std::shared_ptr<StatementList> statements
) : type(type), name(name), statements(statements) {
}

void MethodDecl::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

VariableDecl::VariableDecl(
    std::shared_ptr<GrammaticsType> type,
    const std::string& name
) : type(type), name(name) {
}

void VariableDecl::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}
