#ifndef TYPE_H
#define TYPE_H

#include <string>

class Type {
public:
    virtual ~Type() = default;
    /// Temporary, just for interpreting
    virtual int GetIntValue() = 0;
    virtual std::string GetTypeName() = 0;
};

#endif