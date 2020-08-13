#include <visitors/tree_printer/tree_printer.h>

TreePrinter::TreePrinter(std::ostream& output) : output_(output) {
}

void TreePrinter::PutLine(const std::string& line) {
    output_ << std::string(depth_, ' ') << line << std::endl;
}

void TreePrinter::Visit(std::shared_ptr<ArgumentDecl> arg) {
    ++depth_;
    PutLine("Argument type:");
    arg->type->Accept(shared_from_this());
    PutLine("Argument name:");
    PutLine(std::string(" ") + arg->name);
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<ArgumentDeclList> args) {
    ++depth_;
    if (args->arguments.empty()) {
        PutLine("<empty>");
    } else {
        for (auto arg: args->arguments) {
            arg->Accept(shared_from_this());
            PutLine("");
        }
    }
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<ArgumentValues> vals) {
    ++depth_;
    if (vals->values.empty()) {
        PutLine("<empty>");
    } else {
        for (auto val: vals->values) {
            val->Accept(shared_from_this());
            PutLine("");
        }
    }
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<MethodCall> call) {
    ++depth_;
    PutLine("Call method");
    PutLine("Object:");
    call->object->Accept(shared_from_this());
    PutLine("Method:");
    PutLine(" " + call->method_name);
    PutLine("Arguments:");
    call->values->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<ClassDeclaration> decl) {
    ++depth_;
    PutLine("Class " + decl->name);
    if (!decl->parent.empty()) {
        PutLine("extended from " + decl->parent);
    }
    PutLine("Declarations:");
    decl->declarations->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<ClassDeclList> decls) {
    ++depth_;
    if (decls->declarations.empty()) {
        PutLine("<empty>");
    } else {
        for (auto decl: decls->declarations) {
            decl->Accept(shared_from_this());
            PutLine("");
        }
    }
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<MainClassDecl> decl) {
    ++depth_;
    PutLine("Class name: " + decl->name);
    PutLine("main:");
    decl->main_statements->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<MethodDecl> decl) {
    ++depth_;
    PutLine("Method name: " + decl->name);
    PutLine("Method arguments:");
    decl->arguments->Accept(shared_from_this());
    PutLine("Method statements:");
    decl->statements->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<VariableDecl> decl) {
    ++depth_;
    PutLine("Variable type:");
    decl->type->Accept(shared_from_this());
    PutLine("Variable name:");
    PutLine(" " + decl->name);
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<DeclarationList> decls) {
    ++depth_;
    if (decls->declarations.empty()) {
        PutLine("<empty>");
    } else {
        for (auto decl: decls->declarations) {
            decl->Accept(shared_from_this());
            PutLine("");
        }
    }
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<Allocation> alloc) {
    /// TODO
}

void TreePrinter::Visit(std::shared_ptr<ArrayAllocation> alloc) {
    /// TODO
}

void TreePrinter::Visit(std::shared_ptr<PlusExpr> expr) {
    ++depth_;
    PutLine("Sum of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<MinusExpr> expr) {
    ++depth_;
    PutLine("Difference of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<MulExpr> expr) {
    ++depth_;
    PutLine("Multiplication of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<DivideExpr> expr) {
    ++depth_;
    PutLine("Division of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<RemainderExpr> expr) {
    ++depth_;
    PutLine("Remainder of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<CallExpr> expr) {
    /// TODO
}

void TreePrinter::Visit(std::shared_ptr<GreaterExpr> expr) {
    ++depth_;
    PutLine("Comparison (>) of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<GreaterEqualExpr> expr) {
    ++depth_;
    PutLine("Comparison (>=) of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<LessExpr> expr) {
    ++depth_;
    PutLine("Comparison (<) of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<LessEqualExpr> expr) {
    ++depth_;
    PutLine("Comparison (<=) of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<EqualExpr> expr) {
    ++depth_;
    PutLine("Comparison (==) of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<NotEqualExpr> expr) {
    ++depth_;
    PutLine("Comparison (!=) of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<TrueExpr> expr) {
    ++depth_;
    PutLine("Constant: true");
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<FalseExpr> expr) {
    ++depth_;
    PutLine("Constant: false");
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<NumberExpr> expr) {
    ++depth_;
    PutLine("Constant: " + std::to_string(expr->number));
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<LengthExpr> expr) {
    /// TODO
}

void TreePrinter::Visit(std::shared_ptr<AndExpr> expr) {
    ++depth_;
    PutLine("Logic and of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<OrExpr> expr) {
    ++depth_;
    PutLine("Logic or of two expressions:");
    PutLine("lhs:");
    expr->lhs->Accept(shared_from_this());
    PutLine("rhs:");
    expr->rhs->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<NotExpr> expr) {
    ++depth_;
    PutLine("Logic negation of expression:");
    expr->expr->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<LValueExpr> expr) {
    expr->value->Accept(shared_from_this());
}

void TreePrinter::Visit(std::shared_ptr<ThisExpr> expr) {
    ++depth_;
    PutLine("\"this\" object pointer");
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<Variable> lvalue) {
    ++depth_;
    PutLine("Variable: " + lvalue->name);
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<ArrayElement> lvalue) {
    ++depth_;
    PutLine("Array name: " + lvalue->name);
    PutLine("Element index:");
    lvalue->index->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<Program> prg) {
    depth_ = 0;
    PutLine("Main class:");
    prg->main_class->Accept(shared_from_this());
    PutLine("Other classes:");
    prg->other_classes->Accept(shared_from_this());
}

void TreePrinter::Visit(std::shared_ptr<Assert> stmt) {
    /// TODO
}

void TreePrinter::Visit(std::shared_ptr<Assignment> stmt) {
    ++depth_;
    PutLine("Assignment lvalue:");
    stmt->lvalue->Accept(shared_from_this());
    PutLine("Assignment rvalue:");
    stmt->rvalue->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<CallStatement> stmt) {
    stmt->call->Accept(shared_from_this());
}

void TreePrinter::Visit(std::shared_ptr<ComplexStatement> stmt) {
    stmt->statement_list->Accept(shared_from_this());
}

void TreePrinter::Visit(std::shared_ptr<ConditionalStatement> stmt) {
    ++depth_;
    PutLine("Condition:");
    stmt->condition->Accept(shared_from_this());
    PutLine("If branch:");
    stmt->if_branch->Accept(shared_from_this());
    if (stmt->else_branch) {
        PutLine("Else branch:");
        stmt->else_branch->Accept(shared_from_this());
    }
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<WhileStatement> stmt) {
    ++depth_;
    PutLine("Condition:");
    stmt->condition->Accept(shared_from_this());
    PutLine("Cycle body:");
    stmt->statement->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<PrintStatement> stmt) {
    ++depth_;
    PutLine("Print value of:");
    stmt->value->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<ReturnStatement> stmt) {
    ++depth_;
    PutLine("Return value of:");
    stmt->value->Accept(shared_from_this());
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<VariableStatement> stmt) {
    stmt->variable->Accept(shared_from_this());
}

void TreePrinter::Visit(std::shared_ptr<StatementList> list) {
    ++depth_;
    PutLine("Statement list:");
    if (list->statements.empty()) {
        PutLine(" <empty>");
    } else {
        for (auto stmt: list->statements) {
            stmt->Accept(shared_from_this());
            PutLine("");
        }
    }
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<SimpleType> type) {
    ++depth_;
    PutLine("Simple type: " + type->name);
    --depth_;
}

void TreePrinter::Visit(std::shared_ptr<ArrayType> type) {
    ++depth_;
    PutLine("Array; element type:");
    type->element_type->Accept(shared_from_this());
    --depth_;
}

