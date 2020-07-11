#ifndef RETURN_H
#define RETURN_H

#include <program/expression/expression.h>
#include <program/statement/statement.h>

class ReturnStatement : public Statement, public std::enable_shared_from_this<ReturnStatement> {
public:
    ReturnStatement(std::shared_ptr<Expression> value);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> value;
};

#endif