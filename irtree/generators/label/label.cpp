#include <irtree/generators/label/label.h>

namespace IR {
size_t Label::counter_ = 0;

Label::Label() : name_("Label_" + std::to_string(counter_++)) {
}

Label::Label(const std::string& name) : name_(name) {
}

std::string Label::ToString() const {
    return name_;
}
}
