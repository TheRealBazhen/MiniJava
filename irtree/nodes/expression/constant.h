#ifndef CONSTANT_H
#define CONSTANT_H

#include <irtree/nodes/expression/expression.h>

namespace IR {
class ConstantExpression : public Expression, public std::enable_shared_from_this<ConstantExpression> {
public:
    ConstantExpression(int value);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    int value;
};
}

#endif
