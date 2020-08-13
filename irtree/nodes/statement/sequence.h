#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <irtree/nodes/statement/statement.h>

namespace IR {
class SequenceStatement : public Statement, public std::enable_shared_from_this<SequenceStatement> {
public:
    SequenceStatement(std::shared_ptr<Statement> first, std::shared_ptr<Statement> second);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    std::shared_ptr<Statement> first;
    std::shared_ptr<Statement> second;
};
}

#endif
