#ifndef ASM_SOURCE_H
#define ASM_SOURCE_H

#include <irtree/generators/temporary/temporary.h>

#include <iostream>
#include <memory>

namespace ASM {
class Source {
public:
    virtual ~Source() = default;
    virtual void Print(std::ostream& out) = 0;
};

class Constant : public Source, public std::enable_shared_from_this<Constant> {
public:
    Constant(int value);

    void Print(std::ostream& out) override;

    int value;
};

class Register : public Source, public std::enable_shared_from_this<Register> {
public:
    Register(const IR::Temporary& temp);

    void Print(std::ostream& out) override;

    IR::Temporary temp;
};

class MemoryAccess : public Source, public std::enable_shared_from_this<MemoryAccess> {
public:
    MemoryAccess(const IR::Temporary& base, int offset);

    void Print(std::ostream& out) override;

    IR::Temporary base;
    int offset;
};
}

#endif
