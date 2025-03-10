#ifndef COMPARE
#define COMPARE

#include <program/expression/expression.h>

class GreaterExpr : public Expression, public std::enable_shared_from_this<GreaterExpr> {
public:
    GreaterExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class GreaterEqualExpr : public Expression, public std::enable_shared_from_this<GreaterEqualExpr> {
public:
    GreaterEqualExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class LessExpr : public Expression, public std::enable_shared_from_this<LessExpr> {
public:
    LessExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class LessEqualExpr : public Expression, public std::enable_shared_from_this<LessEqualExpr> {
public:
    LessEqualExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class EqualExpr : public Expression, public std::enable_shared_from_this<EqualExpr> {
public:
    EqualExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class NotEqualExpr : public Expression, public std::enable_shared_from_this<NotEqualExpr> {
public:
    NotEqualExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

#endif
