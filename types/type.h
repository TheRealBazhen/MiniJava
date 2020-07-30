#ifndef TYPE_H
#define TYPE_H

class Type {
public:
    virtual ~Type() = default;
    /// Temporary, just for interpreting
    virtual int GetIntValue() = 0;
};

#endif