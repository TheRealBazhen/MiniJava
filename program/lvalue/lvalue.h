#ifndef LVALUE_H
#define LVALUE_H

#include <program/expression/expression.h>

class LValue : public Node {
};

class Variable : public LValue, public std::enable_shared_from_this<Variable> {
public:
    Variable(const std::string& name);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::string name;
};

class ArrayElement : public LValue, public std::enable_shared_from_this<ArrayElement> {
public:
    ArrayElement(const std::string& name, std::shared_ptr<Expression> index);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::string name;
    std::shared_ptr<Expression> index;
};

#endif