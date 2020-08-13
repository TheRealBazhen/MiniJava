#ifndef IR_EXPRESSION_H
#define IR_EXPRESSION_H

#include <irtree/nodes/node.h>

#include <vector>

namespace IR {
class Expression : public Node {
};

class ExpressionList : public Node, public std::enable_shared_from_this<ExpressionList> {
public:
    void AddExpression(std::shared_ptr<Expression> expr);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    std::vector<std::shared_ptr<Expression>> expressions;
};
}

#endif
