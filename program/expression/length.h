#ifndef LENGTH_H
#define LENGTH_H

#include <program/expression/expression.h>

class LengthExpr : public Expression, public std::enable_shared_from_this<LengthExpr> {
public:
    LengthExpr(std::shared_ptr<Expression> array);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> array;
};

#endif