#ifndef WRAPPER_STATEMENT_H
#define WRAPPER_STATEMENT_H

#include <irtree/wrapper/wrapper.h>

namespace IR {
class StatementWrapper : public SubtreeWrapper {
public:
    StatementWrapper(std::shared_ptr<Statement> statement);

    std::shared_ptr<Expression> ToExpression() override;
    std::shared_ptr<Statement> ToStatement() override;
    std::shared_ptr<Statement> ToConditional(const Label& true_label, const Label& false_label) override;

private:
    std::shared_ptr<Statement> statement_;
};
}

#endif
