#ifndef IR_PRINTER_H
#define IR_PRINTER_H

#include <irtree/visitors/visitor.h>

#include <iostream>
#include <memory>
#include <string>

namespace IR {
class TreePrinter : public Visitor, public std::enable_shared_from_this<TreePrinter> {
public:
    TreePrinter(std::ostream& output, bool highlight_sequences = false);

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
    void PutLine(const std::string& line);

private:
    std::ostream& output_;
    bool highlight_sequences_;
    size_t offset_ = 0;
};
}

#endif
