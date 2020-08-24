#ifndef ASM_MOVEMENTS_H
#define ASM_MOVEMENTS_H

#include <instructions/instruction.h>
#include <instructions/sources/source.h>
#include <irtree/generators/temporary/temporary.h>

#include <memory>

namespace ASM {
class MoveToRegister : public Instruction, std::enable_shared_from_this<MoveToRegister> {
public:
    MoveToRegister(const IR::Temporary& destination, std::shared_ptr<Source> source);

    void Print(std::ostream& out) override;

    IR::Temporary destination;
    std::shared_ptr<Source> source;
};

class MoveToMemory : public Instruction, std::enable_shared_from_this<MoveToMemory> {
public:
    MoveToMemory(std::shared_ptr<MemoryAccess> destination, std::shared_ptr<Source> source);

    void Print(std::ostream& out) override;

    std::shared_ptr<MemoryAccess> destination;
    std::shared_ptr<Source> source;
};
}

#endif
