#include <visitors/interpreter/interpreter.h>

#include <iostream>
#include <stdexcept>

void Interpreter::Visit(std::shared_ptr<ArgumentDecl> arg) {
    /// Not supported
}

void Interpreter::Visit(std::shared_ptr<ArgumentDeclList> args) {
    /// Not supported
}

void Interpreter::Visit(std::shared_ptr<ArgumentValues> vals) {
    /// Not supported
}

void Interpreter::Visit(std::shared_ptr<MethodCall> call) {
    /// Not supported
}

void Interpreter::Visit(std::shared_ptr<ClassDeclaration> decl) {
    /// Not supported
}

void Interpreter::Visit(std::shared_ptr<ClassDeclList> decls) {
    /// Not supported
}

void Interpreter::Visit(std::shared_ptr<MainClassDecl> decl) {
    // Execute main
    decl->main_statements->Accept(shared_from_this());
}

void Interpreter::Visit(std::shared_ptr<MethodDecl> decl) {
    // Not supported
}

void Interpreter::Visit(std::shared_ptr<VariableDecl> decl) {
    variables_[decl->name] = 0;
}

void Interpreter::Visit(std::shared_ptr<DeclarationList> decls) {
    // Not supported
}

void Interpreter::Visit(std::shared_ptr<Allocation> alloc) {
    // Not supported
}

void Interpreter::Visit(std::shared_ptr<ArrayAllocation> alloc) {
    // Not supported
}

void Interpreter::Visit(std::shared_ptr<PlusExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = lhs + rhs;
}

void Interpreter::Visit(std::shared_ptr<MinusExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = lhs - rhs;
}

void Interpreter::Visit(std::shared_ptr<MulExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = lhs * rhs;
}

void Interpreter::Visit(std::shared_ptr<DivideExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = lhs / rhs;
}

void Interpreter::Visit(std::shared_ptr<RemainderExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = lhs % rhs;
}

void Interpreter::Visit(std::shared_ptr<CallExpr> expr) {
    // Not supported
}

void Interpreter::Visit(std::shared_ptr<GreaterExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = (lhs > rhs);
}

void Interpreter::Visit(std::shared_ptr<GreaterEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = (lhs >= rhs);
}

void Interpreter::Visit(std::shared_ptr<LessExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = (lhs < rhs);
}

void Interpreter::Visit(std::shared_ptr<LessEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = (lhs <= rhs);
}

void Interpreter::Visit(std::shared_ptr<EqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = (lhs == rhs);
}

void Interpreter::Visit(std::shared_ptr<NotEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = (lhs != rhs);
}

void Interpreter::Visit(std::shared_ptr<TrueExpr> expr) {
    last_value_ = 1;
}

void Interpreter::Visit(std::shared_ptr<FalseExpr> expr) {
    last_value_ = 0;
}

void Interpreter::Visit(std::shared_ptr<NumberExpr> expr) {
    last_value_ = expr->number;
}

void Interpreter::Visit(std::shared_ptr<LengthExpr> expr) {
    // Not supported
}

void Interpreter::Visit(std::shared_ptr<AndExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = (lhs && rhs);
}

void Interpreter::Visit(std::shared_ptr<OrExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_;
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_;
    last_value_ = (lhs || rhs);
}

void Interpreter::Visit(std::shared_ptr<NotExpr> expr) {
    expr->expr->Accept(shared_from_this());
    last_value_ = !last_value_;
}

void Interpreter::Visit(std::shared_ptr<LValueExpr> expr) {
    expr->value->Accept(shared_from_this());
}

void Interpreter::Visit(std::shared_ptr<ThisExpr> expr) {
    // Not supported
}

void Interpreter::Visit(std::shared_ptr<Variable> lvalue) {
    if (variables_.count(lvalue->name) == 0) {
        throw std::runtime_error("Use of undefined variable: " + lvalue->name);
    }
    last_value_ = variables_[lvalue->name];
}

void Interpreter::Visit(std::shared_ptr<ArrayElement> lvalue) {
    // Not supported
}

void Interpreter::Visit(std::shared_ptr<Program> prg) {
    prg->main_class->Accept(shared_from_this());
}

void Interpreter::Visit(std::shared_ptr<Assert> stmt) {
    stmt->condition->Accept(shared_from_this());
    if (!last_value_) {
        throw std::runtime_error("Assertion failed");
    }
}

void Interpreter::Visit(std::shared_ptr<Assignment> stmt) {
    auto var = std::dynamic_pointer_cast<Variable>(stmt->lvalue);
    if (!var) {
        throw std::runtime_error("Arrays not supported yet");
    }
    if (variables_.count(var->name) == 0) {
        throw std::runtime_error("Use of undefined variable: " + var->name);
    }
    stmt->rvalue->Accept(shared_from_this());
    variables_[var->name] = last_value_;
}

void Interpreter::Visit(std::shared_ptr<CallStatement> stmt) {
    /// Not supported
}

void Interpreter::Visit(std::shared_ptr<ComplexStatement> stmt) {
    stmt->statement_list->Accept(shared_from_this());
}

void Interpreter::Visit(std::shared_ptr<ConditionalStatement> stmt) {
    stmt->condition->Accept(shared_from_this());
    if (last_value_) {
        stmt->if_branch->Accept(shared_from_this());
    } else if (stmt->else_branch) {
        stmt->else_branch->Accept(shared_from_this());
    }
}

void Interpreter::Visit(std::shared_ptr<WhileStatement> stmt) {
    while (true) {
        stmt->condition->Accept(shared_from_this());
        if (!last_value_) {
            break;
        }
        stmt->statement->Accept(shared_from_this());
    }
}

void Interpreter::Visit(std::shared_ptr<PrintStatement> stmt) {
    stmt->value->Accept(shared_from_this());
    std::cout << last_value_ << std::endl;
}

void Interpreter::Visit(std::shared_ptr<ReturnStatement> stmt) {
    /// Not supported
}

void Interpreter::Visit(std::shared_ptr<VariableStatement> stmt) {
    stmt->variable->Accept(shared_from_this());
}

void Interpreter::Visit(std::shared_ptr<StatementList> list) {
    for (auto& stmt: list->statements) {
        stmt->Accept(shared_from_this());
    }
}

void Interpreter::Visit(std::shared_ptr<SimpleType> type) {
    /// Do nothing
}

void Interpreter::Visit(std::shared_ptr<ArrayType> type) {
    /// Do nothing
}
