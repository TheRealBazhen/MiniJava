#ifndef ASM_BINARY_OPERATIONS_H
#define ASM_BINARY_OPERATIONS_H

#include <instructions/instruction.h>
#include <instructions/sources/source.h>
#include <irtree/generators/temporary/temporary.h>

#include <memory>

namespace ASM {
class Add : public Instruction, std::enable_shared_from_this<Add> {
public:
    Add(IR::Temporary destination, std::shared_ptr<Source> source);

    void Print(std::ostream& out) override;

    IR::Temporary destination;
    std::shared_ptr<Source> source;
};

class Sub : public Instruction, std::enable_shared_from_this<Add> {
public:
    Sub(IR::Temporary destination, std::shared_ptr<Source> source);

    void Print(std::ostream& out) override;

    IR::Temporary destination;
    std::shared_ptr<Source> source;
};

class Mul : public Instruction, std::enable_shared_from_this<Mul> {
public:
    Mul(std::shared_ptr<Source> source);

    void Print(std::ostream& out) override;

    std::shared_ptr<Source> source;
};

class Div : public Instruction, std::enable_shared_from_this<Div> {
public:
    Div(std::shared_ptr<Source> divisor);

    void Print(std::ostream& out) override;

    std::shared_ptr<Source> divisor;
};

class Compare : public Instruction, std::enable_shared_from_this<Compare> {
public:
    Compare(std::shared_ptr<Source> lhs, std::shared_ptr<Source> rhs);

    void Print(std::ostream& out) override;

    std::shared_ptr<Source> lhs;
    std::shared_ptr<Source> rhs;
};
}

#endif
