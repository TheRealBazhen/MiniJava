#ifndef ASM_LABEL_H
#define ASM_LABEL_H

#include <instructions/instruction.h>
#include <irtree/generators/label/label.h>

#include <memory>

namespace ASM {
class Label : public Instruction, public std::enable_shared_from_this<Label> {
public:
    Label(const IR::Label& label);

    void Print(std::ostream& out) override;

    IR::Label label;
};
}

#endif
