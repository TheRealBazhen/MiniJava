#include <instructions/label/label.h>

namespace ASM {
Label::Label(const IR::Label& label) : label(label) {
}

void Label::Print(std::ostream& out) {
    /// TODO
}
}
