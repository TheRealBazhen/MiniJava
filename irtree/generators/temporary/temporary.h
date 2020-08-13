#ifndef TEMPORARY_H
#define TEMPORARY_H

#include <string>

namespace IR {
// Virtual index class
class Temporary {
public:
    Temporary();
    explicit Temporary(const std::string& name);

    std::string ToString() const;
private:
    std::string name_;

    static size_t counter_;
};
}

#endif
