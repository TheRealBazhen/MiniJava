#ifndef IR_LINEARIZER_H
#define IR_LINEARIZER_H

#include <irtree/visitors/visitor.h>
#include <irtree/wrapper/wrapper.h>

namespace IR {
class Linearizer : public Visitor, public std::enable_shared_from_this<Linearizer> {
public:
    std::shared_ptr<Statement> GetTree();

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
    std::shared_ptr<IR::SubtreeWrapper> Visit(std::shared_ptr<Node> node);

private:
    std::shared_ptr<IR::SubtreeWrapper> node_;
};
}

#endif
