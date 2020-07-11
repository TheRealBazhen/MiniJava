#ifndef CONSTANTS
#define CONSTANTS

#include <program/expression/expression.h>

class TrueExpr : public BooleanExpr, public std::enable_shared_from_this<TrueExpr> {
public:
    void Accept(std::shared_ptr<Visitor> visitor) override;
};

class FalseExpr : public BooleanExpr, public std::enable_shared_from_this<FalseExpr> {
public:
    void Accept(std::shared_ptr<Visitor> visitor) override;
};

class NumberExpr : public Expression, public std::enable_shared_from_this<NumberExpr> {
public:
    NumberExpr(int number);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    int number;
};

#endif