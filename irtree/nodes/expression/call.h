#ifndef IR_CALL_H
#define IR_CALL_H

#include <irtree/nodes/expression/expression.h>

namespace IR {
class CallExpression : public Expression, public std::enable_shared_from_this<CallExpression> {
public:
    CallExpression(std::shared_ptr<Expression> function, std::shared_ptr<ExpressionList> arguments);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    std::shared_ptr<Expression> function;
    std::shared_ptr<ExpressionList> arguments;
};
}

#endif
