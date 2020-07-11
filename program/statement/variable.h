#ifndef VAR_STATEMENT_H
#define VAR_STATEMENT_H

#include <program/declaration/variable.h>
#include <program/statement/statement.h>

class VariableStatement : public Statement, public std::enable_shared_from_this<VariableStatement> {
public:
    VariableStatement(std::shared_ptr<VariableDecl> variable);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<VariableDecl> variable;
};

#endif