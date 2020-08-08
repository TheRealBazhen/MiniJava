#ifndef LABEL_H
#define LABEL_H

#include <string>

namespace IR {
class Label {
public:
    Label();
    explicit Label(const std::string& name);

    std::string ToString() const;

private:
    std::string name_;

    static size_t counter_;
};
}

#endif
