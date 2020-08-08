#include <irtree/wrapper/statement/statement.h>

namespace IR {
StatementWrapper::StatementWrapper(std::shared_ptr<Statement> statement)
    : statement_(statement) {
}

std::shared_ptr<Expression> StatementWrapper::ToExpression() {
    throw std::runtime_error("Invalid IR tree node conversion: statement to expression");
}

std::shared_ptr<Statement> StatementWrapper::ToStatement() {
    return statement_;
}

std::shared_ptr<Statement> StatementWrapper::ToConditional(const Label& true_label, const Label& false_label) {
    throw std::runtime_error("Invalid IR tree node conversion: statement to conditional");
}
}
