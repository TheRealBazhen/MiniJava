#include <instructions/jumps/call.h>

namespace ASM {
Call::Call(const IR::Label& destination) : destination(destination) {
}

void Call::Print(std::ostream& out) {
    /// TODO
}
}
