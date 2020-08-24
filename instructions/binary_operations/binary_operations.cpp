#include <instructions/binary_operations/binary_operations.h>

namespace ASM {
Add::Add(IR::Temporary destination, std::shared_ptr<Source> source)
    : destination(destination), source(source) {
}

void Add::Print(std::ostream& out) {
    out << "add " << destination.ToString() << ", ";
    source->Print(out);
    out << std::endl;
}

Sub::Sub(IR::Temporary destination, std::shared_ptr<Source> source)
    : destination(destination), source(source) {
}

void Sub::Print(std::ostream& out) {
    out << "sub " << destination.ToString() << ", ";
    source->Print(out);
    out << std::endl;
}

Mul::Mul(std::shared_ptr<Source> source) : source(source) {
}

void Mul::Print(std::ostream& out) {
    out << "imul ";
    source->Print(out);
    out << std::endl;
}

Div::Div(std::shared_ptr<Source> divisor) : divisor(divisor) {
}

void Div::Print(std::ostream& out) {
    out << "imul ";
    divisor->Print(out);
    out << std::endl;
}

Compare::Compare(std::shared_ptr<Source> lhs, std::shared_ptr<Source> rhs) : lhs(lhs), rhs(rhs) {
}

void Compare::Print(std::ostream& out) {
    out << "cmp ";
    lhs->Print(out);
    out << ", ";
    rhs->Print(out);
    out << std::endl;
}
}
