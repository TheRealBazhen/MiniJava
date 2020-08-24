#ifndef ASM_CALL_H
#define ASM_CALL_H

#include <instructions/instruction.h>
#include <irtree/generators/label/label.h>

#include <memory>

namespace ASM {
class Call : public Instruction, public std::enable_shared_from_this<Call> {
public:
    Call(const IR::Label& destination);

    void Print(std::ostream& out) override;

    IR::Label destination;
};
}

#endif
