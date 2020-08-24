#ifndef IR_BLOCK_H
#define IR_BLOCK_H

#include <irtree/nodes/statement/statement.h>
#include <irtree/nodes/statement/label.h>

#include <vector>

namespace IR {
class CodeBlock {
public:
    CodeBlock(const Label& label);
    CodeBlock(std::shared_ptr<LabelStatement> label);

    void AddStatement(std::shared_ptr<Statement> statement);
    std::vector<std::shared_ptr<Statement>> GetStatementList() const;

    Label GetLabel() const;

private:
    std::vector<std::shared_ptr<Statement>> statements_;
    Label label_;
};
}

#endif
