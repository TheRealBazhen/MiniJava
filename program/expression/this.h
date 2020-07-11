#ifndef THIS_EXPR_H
#define THIS_EXPR_H

#include <program/expression/expression.h>

class ThisExpr : public CallableExpr, public std::enable_shared_from_this<ThisExpr> {
public:
    void Accept(std::shared_ptr<Visitor> visitor) override;
};

#endif