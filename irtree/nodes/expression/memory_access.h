#ifndef MEMORY_ACCESS_H
#define MEMORY_ACCESS_H

#include <irtree/nodes/expression/expression.h>

namespace IR {
class MemoryAccessExpression : public Expression, public std::enable_shared_from_this<MemoryAccessExpression> {
public:
    MemoryAccessExpression(std::shared_ptr<Expression> address);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    std::shared_ptr<Expression> address;
};
}

#endif
