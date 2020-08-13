#ifndef LVALUE_EXPR_H
#define LVALUE_EXPR_H

#include <program/expression/expression.h>
#include <program/lvalue/lvalue.h>

class LValueExpr : public Expression, public std::enable_shared_from_this<LValueExpr> {
public:
    LValueExpr(std::shared_ptr<LValue> value);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<LValue> value;
};

#endif