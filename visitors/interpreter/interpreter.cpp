#include <visitors/interpreter/interpreter.h>

#include <iostream>
#include <stdexcept>

Interpreter::Interpreter(const SymbolTree& symbol_tree) : symbol_tree_(symbol_tree) {

}

void Interpreter::Visit(std::shared_ptr<ArgumentDecl> arg) {
}

void Interpreter::Visit(std::shared_ptr<ArgumentDeclList> args) {
}

void Interpreter::Visit(std::shared_ptr<ArgumentValues> vals) {
}

void Interpreter::Visit(std::shared_ptr<MethodCall> call) {
}

void Interpreter::Visit(std::shared_ptr<ClassDeclaration> decl) {
}

void Interpreter::Visit(std::shared_ptr<ClassDeclList> decls) {
}

void Interpreter::Visit(std::shared_ptr<MainClassDecl> decl) {
    // Execute main
    current_layer_ = current_layer_->GetChild("main");
    layer_number_.push_back(0);
    decl->main_statements->Accept(shared_from_this());
}

void Interpreter::Visit(std::shared_ptr<MethodDecl> decl) {
    // Not supported
}

void Interpreter::Visit(std::shared_ptr<VariableDecl> decl) {
}

void Interpreter::Visit(std::shared_ptr<DeclarationList> decls) {
}

void Interpreter::Visit(std::shared_ptr<Allocation> alloc) {
}

void Interpreter::Visit(std::shared_ptr<ArrayAllocation> alloc) {
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
}

void Interpreter::Visit(std::shared_ptr<Variable> lvalue) {
    last_value_ = current_layer_->GetValue(lvalue->name)->GetIntValue();
}

void Interpreter::Visit(std::shared_ptr<ArrayElement> lvalue) {
}

void Interpreter::Visit(std::shared_ptr<Program> prg) {
    current_layer_ = symbol_tree_.GetRoot();
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
    stmt->rvalue->Accept(shared_from_this());
    current_layer_->SetValue(var->name, std::make_shared<IntegerType>(last_value_));
}

void Interpreter::Visit(std::shared_ptr<CallStatement> stmt) {
    /// Not supported
}

void Interpreter::Visit(std::shared_ptr<ComplexStatement> stmt) {
    current_layer_ = current_layer_->GetChild(layer_number_.back());
    ++layer_number_.back();
    layer_number_.push_back(0);
    stmt->statement_list->Accept(shared_from_this());
    layer_number_.pop_back();
    current_layer_ = current_layer_->GetParent();
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
    size_t old_layer_number = layer_number_.back();
    while (true) {
        stmt->condition->Accept(shared_from_this());
        if (!last_value_) {
            break;
        }
        layer_number_.back() = old_layer_number;
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
