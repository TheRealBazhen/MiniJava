#ifndef IR_BLOCK_BUILDER_H
#define IR_BLOCK_BUILDER_H

#include <irtree/blocks/block_sequence/block_sequence.h>
#include <irtree/visitors/visitor.h>

namespace IR {
class BlockBuilder : public Visitor, public std::enable_shared_from_this<BlockBuilder> {
public:
    BlockSequence GetBlocks();

    void Visit(std::shared_ptr<BinaryOperationExpression> expr) override;
    void Visit(std::shared_ptr<CallExpression> expr) override;
    void Visit(std::shared_ptr<ConstantExpression> expr) override;
    void Visit(std::shared_ptr<ESEQExpression> expr) override;
    void Visit(std::shared_ptr<MemoryAccessExpression> expr) override;
    void Visit(std::shared_ptr<NameExpression> expr) override;
    void Visit(std::shared_ptr<TemporaryExpression> expr) override;
    void Visit(std::shared_ptr<ConditionalJumpStatement> stmt) override;
    void Visit(std::shared_ptr<ExpressionStatement> stmt) override;
    void Visit(std::shared_ptr<JumpStatement> stmt) override;
    void Visit(std::shared_ptr<LabelStatement> stmt) override;
    void Visit(std::shared_ptr<MoveStatement> stmt) override;
    void Visit(std::shared_ptr<SequenceStatement> stmt) override;
    void Visit(std::shared_ptr<ExpressionList> list) override;

private:
    BlockSequence blocks_;
};
}

#endif
