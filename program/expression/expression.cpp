#include <program/visitor.h>

Allocation::Allocation(std::shared_ptr<SimpleType> type) : type(type) {
}

void Allocation::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

ArrayAllocation::ArrayAllocation(
    std::shared_ptr<SimpleType> type,
    std::shared_ptr<Expression> count
) : type(type), count(count) {
}

void ArrayAllocation::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

PlusExpr::PlusExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void PlusExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

MinusExpr::MinusExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void MinusExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

MulExpr::MulExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void MulExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

DivideExpr::DivideExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void DivideExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

RemainderExpr::RemainderExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void RemainderExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

CallExpr::CallExpr(std::shared_ptr<MethodCall> call) : call(call) {
}

void CallExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

GreaterExpr::GreaterExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void GreaterExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

GreaterEqualExpr::GreaterEqualExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void GreaterEqualExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

LessExpr::LessExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void LessExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

LessEqualExpr::LessEqualExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void LessEqualExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

EqualExpr::EqualExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void EqualExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

NotEqualExpr::NotEqualExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void NotEqualExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

AndExpr::AndExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void AndExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

OrExpr::OrExpr(
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : lhs(lhs), rhs(rhs) {
}

void OrExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

NotExpr::NotExpr(
    std::shared_ptr<Expression> expr
) : expr(expr) {
}

void NotExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

void TrueExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

void FalseExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

NumberExpr::NumberExpr(int number) : number(number) {
}

void NumberExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

LengthExpr::LengthExpr(std::shared_ptr<Expression> array) : array(array) {
}

void LengthExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

LValueExpr::LValueExpr(std::shared_ptr<LValue> value) : value(value) {
}

void LValueExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

void ThisExpr::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}
