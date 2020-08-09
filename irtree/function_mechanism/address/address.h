#ifndef IR_ADDRESS_H
#define IR_ADDRESS_H

#include <irtree/generators/temporary/temporary.h>
#include <irtree/nodes/expression/expression.h>

namespace IR {
class Frame;

class Address {
public:
    virtual ~Address() = default;

    virtual std::shared_ptr<Expression> ToExpression() = 0;
};

class RegisterAddress : public Address {
public:
    RegisterAddress(const Temporary& temp);

    std::shared_ptr<Expression> ToExpression() override;

private:
    Temporary temp_;
};

class OffsetAddress : public Address {
public:
    OffsetAddress(std::shared_ptr<Address> base_address, size_t offset);

    std::shared_ptr<Expression> ToExpression() override;

private:
    std::shared_ptr<Address> base_address_;
    size_t offset_;
};
}

#endif
