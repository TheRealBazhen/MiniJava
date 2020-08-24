#ifndef ASM_JUMP_H
#define ASM_JUMP_H

#include <instructions/instruction.h>
#include <irtree/generators/label/label.h>

#include <memory>

namespace ASM {
class Jump : public Instruction, public std::enable_shared_from_this<Jump> {
public:
    Jump(const IR::Label& destination);

    void Print(std::ostream& out) override;

    IR::Label destination;
};
}

#endif
