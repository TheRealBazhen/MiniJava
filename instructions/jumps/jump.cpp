#include <instructions/jumps/jump.h>

namespace ASM {
Jump::Jump(const IR::Label& destination) : destination(destination) {
}

void Jump::Print(std::ostream& out) {
    /// TODO
}
}