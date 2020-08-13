#ifndef WRAPPER_H
#define WRAPPER_H

#include <irtree/generators/label/label.h>
#include <irtree/nodes/expression/expression.h>
#include <irtree/nodes/statement/statement.h>

namespace IR {
class SubtreeWrapper {
public:
    virtual ~SubtreeWrapper() = default;

    virtual std::shared_ptr<Expression> ToExpression() = 0;
    virtual std::shared_ptr<Statement> ToStatement() = 0;
    virtual std::shared_ptr<Statement> ToConditional(const Label& true_label, const Label& false_label) = 0;
};
}

#endif
