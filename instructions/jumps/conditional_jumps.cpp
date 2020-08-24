#include <instructions/jumps/conditional_jumps.h>

namespace ASM {
ConditionalJump::ConditionalJump(IR::ConditionalOperatorType type, const IR::Label& destination)
    : type(type), destination(destination) {
}

void ConditionalJump::Print(std::ostream& out) {
    std::string suffix;
    if (type == IR::ConditionalOperatorType::LE) {
        suffix = "jle";
    } else if (type == IR::ConditionalOperatorType::LT) {
        suffix = "jl";
    } else if (type == IR::ConditionalOperatorType::GE) {
        suffix = "jge";
    } else if (type == IR::ConditionalOperatorType::GT) {
        suffix = "jg";
    } else if (type == IR::ConditionalOperatorType::EQ) {
        suffix = "jz";
    } else {
        suffix = "jnz";
    }
    out << suffix << " " << destination.ToString() << std::endl;
}
}
