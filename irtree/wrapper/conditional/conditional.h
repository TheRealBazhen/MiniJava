#ifndef WRAPPER_CONDITIONAL_H
#define WRAPPER_CONDITIONAL_H

#include <irtree/wrapper/wrapper.h>

namespace IR {
class ConditionalWrapper : public SubtreeWrapper {
public:
    std::shared_ptr<Expression> ToExpression() override;
    std::shared_ptr<Statement> ToStatement() override;
};
}

#endif
