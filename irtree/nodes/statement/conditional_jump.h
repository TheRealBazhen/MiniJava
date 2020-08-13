#ifndef CONDITIONAL_JUMP_H
#define CONDITIONAL_JUMP_H

#include <irtree/nodes/statement/statement.h>
#include <irtree/nodes/expression/expression.h>
#include <irtree/generators/label/label.h>

namespace IR {
enum class ConditionalOperatorType {
    EQ, NE, LT, GT, LE, GE
};

class ConditionalJumpStatement : public Statement, public std::enable_shared_from_this<ConditionalJumpStatement> {
public:
    ConditionalJumpStatement(
        ConditionalOperatorType cond_operator,
        std::shared_ptr<Expression> lhs,
        std::shared_ptr<Expression> rhs,
        const Label& true_label,
        const Label& false_label
    );

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    ConditionalOperatorType cond_operator;
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
    Label true_label;
    Label false_label;
};
}

#endif
