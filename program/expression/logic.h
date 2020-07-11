#ifndef LOGIC_H
#define LOGIC_H

#include <program/expression/expression.h>

class OrExpr : public BooleanExpr, public std::enable_shared_from_this<OrExpr> {
public:
    OrExpr(std::shared_ptr<BooleanExpr> lhs, std::shared_ptr<BooleanExpr> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<BooleanExpr> lhs;
    std::shared_ptr<BooleanExpr> rhs;
};

class AndExpr : public BooleanExpr, public std::enable_shared_from_this<AndExpr> {
public:
    AndExpr(std::shared_ptr<BooleanExpr> lhs, std::shared_ptr<BooleanExpr> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<BooleanExpr> lhs;
    std::shared_ptr<BooleanExpr> rhs;
};

class NotExpr : public BooleanExpr, public std::enable_shared_from_this<NotExpr> {
public:
    NotExpr(std::shared_ptr<BooleanExpr> expr);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<BooleanExpr> expr;
};

#endif
