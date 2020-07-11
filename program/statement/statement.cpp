#include <program/visitor.h>

Assert::Assert(std::shared_ptr<BooleanExpr> condition) : condition(condition) {
}

void Assert::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

Assignment::Assignment(
    std::shared_ptr<LValue> lvalue,
    std::shared_ptr<Expression> rvalue
) : lvalue(lvalue), rvalue(rvalue) {
}

void Assignment::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

CallStatement::CallStatement(std::shared_ptr<MethodCall> call) : call(call) {
}

void CallStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

ConditionalStatement::ConditionalStatement(
    std::shared_ptr<BooleanExpr> condition,
    std::shared_ptr<Statement> if_branch,
    std::shared_ptr<Statement> else_branch
) : condition(condition), if_branch(if_branch), else_branch(else_branch) {
}

void ConditionalStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

WhileStatement::WhileStatement(
    std::shared_ptr<BooleanExpr> condition,
    std::shared_ptr<Statement> statement
) : condition(condition), statement(statement) {
}

void WhileStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

PrintStatement::PrintStatement(std::shared_ptr<Expression> value) : value(value) {
}

void PrintStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

ReturnStatement::ReturnStatement(std::shared_ptr<Expression> value) : value(value) {
}

void ReturnStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

VariableStatement::VariableStatement(std::shared_ptr<VariableDecl> variable) : variable(variable) {
}

void VariableStatement::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}

void StatementList::AddStatement(std::shared_ptr<Statement> statement) {
    statements.push_back(statement);
}

void StatementList::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}
