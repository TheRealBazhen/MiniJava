#ifndef ARIPHMETICS
#define ARIPHMETICS

#include <program/expression/expression.h>

class PlusExpr : public Expression, public std::enable_shared_from_this<PlusExpr> {
public:
    PlusExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class MinusExpr : public Expression, public std::enable_shared_from_this<MinusExpr> {
public:
    MinusExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class MulExpr : public Expression, public std::enable_shared_from_this<MulExpr> {
public:
    MulExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class DivideExpr : public Expression, public std::enable_shared_from_this<DivideExpr> {
public:
    DivideExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

class RemainderExpr : public Expression, public std::enable_shared_from_this<RemainderExpr> {
public:
    RemainderExpr(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

#endif
