#ifndef MOVE_H
#define MOVE_H

#include <irtree/nodes/statement/statement.h>
#include <irtree/nodes/expression/expression.h>

namespace IR {
class MoveStatement : public Statement, public std::enable_shared_from_this<MoveStatement> {
public:
    MoveStatement(std::shared_ptr<Expression> destination, std::shared_ptr<Expression> source);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    std::shared_ptr<Expression> destination;
    std::shared_ptr<Expression> source;
};
}

#endif
