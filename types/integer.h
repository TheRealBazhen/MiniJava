#ifndef INTEGER_H
#define INTEGER_H

#include <types/type.h>

class IntegerType : public Type {
public:
    IntegerType(int value = 0);

    int GetIntValue() override;

    std::string GetTypeName() override;
private:
    int value_;
};

#endif