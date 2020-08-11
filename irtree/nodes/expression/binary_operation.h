#ifndef BINARY_OPERATION_H
#define BINARY_OPERATION_H

#include <irtree/nodes/expression/expression.h>

namespace IR {
enum class BinaryOperaionType {
    PLUS, MINUS, MUL, DIV, REM, AND, OR
};

class BinaryOperationExpression : public Expression, public std::enable_shared_from_this<BinaryOperationExpression> {
public:
    BinaryOperationExpression(
        BinaryOperaionType type,
        std::shared_ptr<Expression> lhs,
        std::shared_ptr<Expression> rhs
    );

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    BinaryOperaionType type;
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};
}

#endif
