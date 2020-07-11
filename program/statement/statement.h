#ifndef STATEMENT
#define STATEMENT

#include <program/node.h>

#include <vector>

class Statement : public Node {
};

class StatementList : public Statement, public std::enable_shared_from_this<StatementList> {
public:
    void AddStatement(std::shared_ptr<Statement> statement);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::vector<std::shared_ptr<Statement>> statements;
};

#endif