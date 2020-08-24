#include <irtree/visitors/block_builder/block_builder.h>

namespace IR {
BlockSequence BlockBuilder::GetBlocks() {
    return blocks_;
}

void BlockBuilder::Visit(std::shared_ptr<BinaryOperationExpression> expr) {
    // Ignore
}

void BlockBuilder::Visit(std::shared_ptr<CallExpression> expr) {
    // Ignore
}

void BlockBuilder::Visit(std::shared_ptr<ConstantExpression> expr) {
    // Ignore
}

void BlockBuilder::Visit(std::shared_ptr<ESEQExpression> expr) {
    // Ignore
}

void BlockBuilder::Visit(std::shared_ptr<MemoryAccessExpression> expr) {
    // Ignore
}

void BlockBuilder::Visit(std::shared_ptr<NameExpression> expr) {
    // Ignore
}

void BlockBuilder::Visit(std::shared_ptr<TemporaryExpression> expr) {
    // Ignore
}

void BlockBuilder::Visit(std::shared_ptr<ConditionalJumpStatement> stmt) {
    blocks_.AddStatement(stmt);
}

void BlockBuilder::Visit(std::shared_ptr<ExpressionStatement> stmt) {
    blocks_.AddStatement(stmt);
}

void BlockBuilder::Visit(std::shared_ptr<JumpStatement> stmt) {
    blocks_.AddStatement(stmt);
}

void BlockBuilder::Visit(std::shared_ptr<LabelStatement> stmt) {
    blocks_.AddStatement(stmt);
}

void BlockBuilder::Visit(std::shared_ptr<MoveStatement> stmt) {
    blocks_.AddStatement(stmt);
}

void BlockBuilder::Visit(std::shared_ptr<SequenceStatement> stmt) {
    stmt->first->Accept(shared_from_this());
    stmt->second->Accept(shared_from_this());
}

void BlockBuilder::Visit(std::shared_ptr<ExpressionList> list) {
    // Ignore
}

}
