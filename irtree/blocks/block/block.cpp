#include <irtree/blocks/block/block.h>

namespace IR {
CodeBlock::CodeBlock(const Label& label) : label_(label) {
    statements_.push_back(std::make_shared<LabelStatement>(label));
}

CodeBlock::CodeBlock(std::shared_ptr<LabelStatement> label) : label_(label->label) {
    statements_.push_back(label);
}

void CodeBlock::AddStatement(std::shared_ptr<Statement> statement) {
    statements_.push_back(statement);
}

std::vector<std::shared_ptr<Statement>> CodeBlock::GetStatementList() const {
    return statements_;
}

Label CodeBlock::GetLabel() const {
    return label_;
}
}
