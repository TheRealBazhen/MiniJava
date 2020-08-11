#ifndef CONDITIONAL_H
#define CONDITIONAL_H

#include <program/expression/expression.h>
#include <program/statement/statement.h>

class ConditionalStatement : public Statement, public std::enable_shared_from_this<ConditionalStatement> {
public:
    ConditionalStatement(
        std::shared_ptr<Expression> condition,
        std::shared_ptr<Statement> if_branch,
        std::shared_ptr<Statement> else_branch = nullptr
    );

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> condition;
    std::shared_ptr<Statement> if_branch;
    std::shared_ptr<Statement> else_branch;
};

class WhileStatement : public Statement, public std::enable_shared_from_this<WhileStatement> {
public:
    WhileStatement(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> statement);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> condition;
    std::shared_ptr<Statement> statement;
};

#endif