#ifndef ASM_CONDITIONAL_JUMPS_H
#define ASM_CONDITIONAL_JUMPS_H

#include <instructions/instruction.h>
#include <irtree/generators/label/label.h>
#include <irtree/nodes/statement/conditional_jump.h>

#include <memory>

namespace ASM {
class ConditionalJump : public Instruction, std::enable_shared_from_this<ConditionalJump> {
public:
    ConditionalJump(IR::ConditionalOperatorType type, const IR::Label& destination);

    void Print(std::ostream& out) override;

    IR::ConditionalOperatorType type;
    IR::Label destination;
};
}

#endif
