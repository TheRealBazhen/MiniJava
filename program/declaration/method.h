#ifndef METHOD_DECL
#define METHOD_DECL

#include <program/arguments/declaraion.h>
#include <program/declaration/declaration.h>
#include <program/statement/statement.h>
#include <program/types/types.h>

#include <string>

class MethodDecl : public Declaration, public std::enable_shared_from_this<MethodDecl> {
public:
    MethodDecl(std::shared_ptr<GrammaticsType> type, const std::string& name, std::shared_ptr<ArgumentDeclList> args, std::shared_ptr<StatementList> statements);
    MethodDecl(std::shared_ptr<GrammaticsType> type, const std::string& name, std::shared_ptr<StatementList> statements);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::string name;
    std::shared_ptr<GrammaticsType> type;
    std::shared_ptr<ArgumentDeclList> arguments;
    std::shared_ptr<StatementList> statements;
};

#endif