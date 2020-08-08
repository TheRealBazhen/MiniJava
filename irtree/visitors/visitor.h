#ifndef IR_VISITOR_H
#define IR_VISITOR_H

#include <irtree/nodes/expression/binary_operation.h>
#include <irtree/nodes/expression/call.h>
#include <irtree/nodes/expression/constant.h>
#include <irtree/nodes/expression/eseq.h>
#include <irtree/nodes/expression/memory_access.h>
#include <irtree/nodes/expression/name.h>
#include <irtree/nodes/expression/temporary.h>
#include <irtree/nodes/statement/conditional_jump.h>
#include <irtree/nodes/statement/expression.h>
#include <irtree/nodes/statement/jump.h>
#include <irtree/nodes/statement/label.h>
#include <irtree/nodes/statement/move.h>
#include <irtree/nodes/statement/sequence.h>

#include <memory>

namespace IR {
class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void Visit(std::shared_ptr<BinaryOperationExpression> expr) = 0;
    virtual void Visit(std::shared_ptr<CallExpression> expr) = 0;
    virtual void Visit(std::shared_ptr<ConstantExpression> expr) = 0;
    virtual void Visit(std::shared_ptr<ESEQExpression> expr) = 0;
    virtual void Visit(std::shared_ptr<MemoryAccessExpression> expr) = 0;
    virtual void Visit(std::shared_ptr<NameExpression> expr) = 0;
    virtual void Visit(std::shared_ptr<TemporaryExpression> expr) = 0;
    virtual void Visit(std::shared_ptr<ConditionalJumpStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<ExpressionStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<JumpStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<LabelStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<MoveStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<SequenceStatement> stmt) = 0;
};
}

#endif
