#ifndef STATEMENT_EXPRESSION_H
#define STATEMENT_EXPRESSION_H

#include <irtree/nodes/statement/statement.h>
#include <irtree/nodes/expression/expression.h>

namespace IR {
class ExpressionStatement : public Statement, public std::enable_shared_from_this<ExpressionStatement> {
public:
    ExpressionStatement(std::shared_ptr<Expression> expression);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    std::shared_ptr<Expression> expression;
};
}

#endif
