#include <instructions/binary_operations/binary_operations.h>

namespace ASM {
Add::Add(IR::Temporary destination, std::shared_ptr<Source> source)
    : destination(destination), source(source) {
}

void Add::Print(std::ostream& out) {
    /// TODO
}

Sub::Sub(IR::Temporary destination, std::shared_ptr<Source> source)
    : destination(destination), source(source) {
}

void Sub::Print(std::ostream& out) {
    /// TODO
}

Mul::Mul(std::shared_ptr<Source> source) : source(source) {
}

void Mul::Print(std::ostream& out) {
    /// TODO
}

Div::Div(std::shared_ptr<Source> divisor) : divisor(divisor) {
}

void Div::Print(std::ostream& out) {
    /// TODO
}

Compare::Compare(std::shared_ptr<Source> lhs, std::shared_ptr<Source> rhs) : lhs(lhs), rhs(rhs) {
}

void Compare::Print(std::ostream& out) {
    /// TODO
}
}
