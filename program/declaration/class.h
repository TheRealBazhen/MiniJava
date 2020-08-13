#ifndef CLASS_DECLARATION
#define CLASS_DECLARATION

#include <program/declaration/declaration.h>
#include <program/statement/statement.h>

#include <string>

class ClassDeclaration : public Node, public std::enable_shared_from_this<ClassDeclaration> {
public:
    ClassDeclaration(const std::string& name, const std::string& parent, std::shared_ptr<DeclarationList> declarations);
    ClassDeclaration(const std::string& name, std::shared_ptr<DeclarationList> declarations);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::string name;
    std::string parent;
    std::shared_ptr<DeclarationList> declarations;
};

class ClassDeclList : public Node, public std::enable_shared_from_this<ClassDeclList> {
public:
    void AddDeclaration(std::shared_ptr<ClassDeclaration> declaration);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::vector<std::shared_ptr<ClassDeclaration>> declarations;
};

class MainClassDecl : public Node, public std::enable_shared_from_this<MainClassDecl> {
public:
    MainClassDecl(const std::string& name, std::shared_ptr<StatementList> main_statements);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::string name;
    std::shared_ptr<StatementList> main_statements;
};


#endif