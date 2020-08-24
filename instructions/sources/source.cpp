#include <instructions/sources/source.h>

namespace ASM {
Constant::Constant(int value) : value(value) {
}

void Constant::Print(std::ostream& out) {
    out << value;
}

Register::Register(const IR::Temporary& temp) : temp(temp) {
}

void Register::Print(std::ostream& out) {
    out << temp.ToString();
}

MemoryAccess::MemoryAccess(const IR::Temporary& base, int offset) : base(base), offset(offset) {
}

void MemoryAccess::Print(std::ostream& out) {
    out << "[" << base.ToString();
    if (offset != 0) {
        out << " + " << offset;
    }
    out << "]";
}
}
