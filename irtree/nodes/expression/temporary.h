#ifndef IR_TEMPORARY_H
#define IR_TEMPORARY_H

#include <irtree/nodes/expression/expression.h>
#include <irtree/generators/temporary/temporary.h>

namespace IR {
class TemporaryExpression : public Expression, public std::enable_shared_from_this<TemporaryExpression> {
public:
    TemporaryExpression(const Temporary& temp);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    Temporary temp;
};
}

#endif
