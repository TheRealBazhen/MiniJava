#ifndef ASSINGMENT_H
#define ASSINGMENT_H

#include <program/expression/expression.h>
#include <program/lvalue/lvalue.h>
#include <program/statement/statement.h>

class Assignment : public Statement, public std::enable_shared_from_this<Assignment> {
public:
    Assignment(std::shared_ptr<LValue> lvalue, std::shared_ptr<Expression> rvalue);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<LValue> lvalue;
    std::shared_ptr<Expression> rvalue;
};

#endif