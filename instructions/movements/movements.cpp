#include <instructions/movements/movements.h>

namespace ASM {
MoveToMemory::MoveToMemory(std::shared_ptr<MemoryAccess> destination, std::shared_ptr<Source> source)
    : destination(destination), source(source) {
}

void MoveToMemory::Print(std::ostream& out) {
    out << "mov ";
    destination->Print(out);
    out << ", ";
    source->Print(out);
    out << std::endl;
}

MoveToRegister::MoveToRegister(const IR::Temporary& destination, std::shared_ptr<Source> source)
    : destination(destination), source(source) {
}

void MoveToRegister::Print(std::ostream& out) {
    out << "mov " << destination.ToString() << ", ";
    source->Print(out);
    out << std::endl;
}
}
