#ifndef INSTRUCTION_SELECTOR_H
#define INSTRUCTION_SELECTOR_H

#include <instructions/instruction.h>
#include <instructions/sources/source.h>
#include <irtree/visitors/visitor.h>

namespace IR {
class InstructionSelector : public Visitor, public std::enable_shared_from_this<InstructionSelector> {
public:
    InstructionSelector();

    std::vector<std::shared_ptr<ASM::Instruction>> GetInstructions() const;

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
    std::shared_ptr<ASM::Source> GetSource(std::shared_ptr<Expression> expr);

    Temporary Visit(std::shared_ptr<Node> node);

private:
    std::vector<std::shared_ptr<ASM::Instruction>> instructions_;
    Temporary result_register_;

    Temporary eax_;
    Temporary edx_;
};
}

#endif
