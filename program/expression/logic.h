#ifndef LOGIC_H
#define LOGIC_H

#include <program/expression/expression.h>

class OrExpr : public Expression, public std::enable_shared_from_this<OrExpr> {
public:
    OrExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class AndExpr : public Expression, public std::enable_shared_from_this<AndExpr> {
public:
    AndExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class NotExpr : public Expression, public std::enable_shared_from_this<NotExpr> {
public:
    NotExpr(std::shared_ptr<Expression> expr);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> expr;
};

#endif
