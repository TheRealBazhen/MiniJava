#include <irtree/visitors/visitor.h>

namespace IR {
BinaryOperationExpression::BinaryOperationExpression(
    BinaryOperaionType type,
    std::shared_ptr<Expression> lhs,
    std::shared_ptr<Expression> rhs
) : type(type), lhs(lhs), rhs(rhs){
}

void BinaryOperationExpression::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

CallExpression::CallExpression(std::shared_ptr<Expression> function, std::shared_ptr<ExpressionList> arguments)
    : function(function), arguments(arguments){
}

void CallExpression::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

ConstantExpression::ConstantExpression(int value) : value(value) {
}

void ConstantExpression::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

ESEQExpression::ESEQExpression(std::shared_ptr<Statement> side_effect, std::shared_ptr<Expression> value)
    : side_effect(side_effect), value(value) {
}

void ESEQExpression::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

MemoryAccessExpression::MemoryAccessExpression(std::shared_ptr<Expression> address) : address(address) {
}

void MemoryAccessExpression::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

NameExpression::NameExpression(const Label& label) : label(label) {
}

void NameExpression::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

TemporaryExpression::TemporaryExpression(const Temporary& temp) : temp(temp) {
}

void TemporaryExpression::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}
}
