#ifndef STATEMENT_LABEL_H
#define STATEMENT_LABEL_H

#include <irtree/nodes/statement/statement.h>
#include <irtree/generators/label/label.h>

namespace IR {
class LabelStatement : public Statement, public std::enable_shared_from_this<LabelStatement> {
public:
    LabelStatement(const Label& label);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    Label label;
};
}

#endif