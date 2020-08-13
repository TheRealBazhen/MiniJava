#include <irtree/generators/temporary/temporary.h>

namespace IR {
size_t Temporary::counter_ = 0;

Temporary::Temporary() : name_("%" + std::to_string(counter_++)) {
}

Temporary::Temporary(const std::string& name) : name_(name) {
}

std::string Temporary::ToString() const {
    return name_;
}
}
