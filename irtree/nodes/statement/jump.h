#ifndef JUMP_H
#define JUMP_H

#include <irtree/nodes/statement/statement.h>
#include <irtree/nodes/expression/expression.h>

namespace IR {
class JumpStatement : public Statement, public std::enable_shared_from_this<JumpStatement> {
public:
    JumpStatement(std::shared_ptr<Expression> address);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    std::shared_ptr<Expression> address;
};
}

#endif
