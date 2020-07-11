#ifndef ASSERT_H
#define ASSERT_H

#include <program/expression/expression.h>
#include <program/statement/statement.h>

class Assert : public Statement, public std::enable_shared_from_this<Assert> {
public:
    Assert(std::shared_ptr<BooleanExpr> condition);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<BooleanExpr> condition;
};

#endif