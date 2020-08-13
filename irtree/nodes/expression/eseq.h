#ifndef ESEQ_H
#define ESEQ_H

#include <irtree/nodes/expression/expression.h>
#include <irtree/nodes/statement/statement.h>

namespace IR {
class ESEQExpression : public Expression, public std::enable_shared_from_this<ESEQExpression> {
public:
    ESEQExpression(std::shared_ptr<Statement> side_effect, std::shared_ptr<Expression> value);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    std::shared_ptr<Statement> side_effect;
    std::shared_ptr<Expression> value;
};
}

#endif
