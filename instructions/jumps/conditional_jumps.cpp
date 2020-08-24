#include <instructions/jumps/conditional_jumps.h>

namespace ASM {
ConditionalJump::ConditionalJump(IR::ConditionalOperatorType type, const IR::Label& destination)
    : type(type), destination(destination) {
}

void ConditionalJump::Print(std::ostream& out) {
    /// TODO
}
}
