#ifndef ALLOCATION
#define ALLOCATION

#include <program/expression/expression.h>
#include <program/types/types.h>

class Allocation : public CallableExpr, public std::enable_shared_from_this<Allocation> {
public:
    Allocation(std::shared_ptr<SimpleType> type);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<SimpleType> type;
};

class ArrayAllocation : public CallableExpr, public std::enable_shared_from_this<ArrayAllocation> {
public:
    ArrayAllocation(std::shared_ptr<SimpleType> type, std::shared_ptr<Expression> count);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<SimpleType> type;
    std::shared_ptr<Expression> count;
};

#endif