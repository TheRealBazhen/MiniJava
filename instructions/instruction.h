#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>

namespace ASM {
class Instruction {
public:
    virtual ~Instruction() = default;
    virtual void Print(std::ostream& out) = 0;
};
}

#endif