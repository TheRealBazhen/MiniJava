#include <visitors/symbol_tree_builder/symbol_tree_builder.h>

void SymbolTreeBuilder::Visit(std::shared_ptr<ArgumentDecl> arg) {
    if (mode_ == SECOND_PASS) {
        current_layer_->DeclareSymbol(arg->name, std::make_shared<IntegerType>());
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ArgumentDeclList> args) {
    for (auto decl : args->arguments) {
        decl->Accept(shared_from_this());
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ArgumentValues> vals) {
    /// Not supported
}

void SymbolTreeBuilder::Visit(std::shared_ptr<MethodCall> call) {
    if (mode_ == SECOND_PASS) {
        auto expr = std::dynamic_pointer_cast<LValueExpr>(call->object);
        std::string class_name;
        if (expr) {
            auto var = std::dynamic_pointer_cast<Variable>(expr->value);
            if (var) {
                auto object = std::dynamic_pointer_cast<ClassType>(current_layer_->GetValue(var->name));
                if (!object) {
                    throw std::runtime_error("Method can be called only if object is class");
                }
                class_name = object->GetClassName();
            } 
        } else if (std::dynamic_pointer_cast<ThisExpr>(call->object)) {
            class_name = current_class_name_;
        } else {
            throw std::runtime_error("Method can be called only from variable or 'this'");
        }

        std::shared_ptr<Type> symb;
        try {
            symb = root_->GetChild(class_name)->GetValue(call->method_name);
        } catch (...) {
            throw std::runtime_error("Method '" + call->method_name + "' of class '" + class_name + "' not found");
        }
        
        if (!std::dynamic_pointer_cast<FunctionType>(symb)) {
            throw std::runtime_error(call->method_name + " is not a method");
        }
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ClassDeclaration> decl) {
    if (mode_ == FIRST_PASS) {
        current_layer_ = std::make_shared<SymbolLayer>(root_);
        current_class_name_ = decl->name;
        root_->AddChild(current_layer_);
        root_->AddAccociation(current_layer_, decl->name);
        cur_description_.field_names.clear();
        decl->declarations->Accept(shared_from_this());
        classes_descriptions_[decl->name] = cur_description_;
        current_layer_ = root_;
    } else {
        current_layer_ = root_->GetChild(decl->name);
        current_class_name_ = decl->name;
        decl->declarations->Accept(shared_from_this());
        current_layer_ = root_;
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ClassDeclList> decls) {
    for (auto decl : decls->declarations) {
        decl->Accept(shared_from_this());
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<MainClassDecl> decl) {
    current_layer_ = std::make_shared<SymbolLayer>(root_);
    root_->AddChild(current_layer_);
    root_->AddAccociation(current_layer_, "main");
    decl->main_statements->Accept(shared_from_this());
    current_layer_ = root_;
}

void SymbolTreeBuilder::Visit(std::shared_ptr<MethodDecl> decl) {
    if (mode_ == SECOND_PASS) {
        current_layer_ = std::make_shared<SymbolLayer>(current_layer_);
        current_layer_->GetParent()->AddChild(current_layer_);
        current_layer_->GetParent()->AddAccociation(current_layer_, decl->name);
        decl->arguments->Accept(shared_from_this());
        decl->statements->Accept(shared_from_this());
        current_layer_ = current_layer_->GetParent();
    } else {
        current_layer_->DeclareSymbol(decl->name, std::make_shared<FunctionType>(decl));
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<VariableDecl> decl) {
    if (mode_ == FIRST_PASS) {
        cur_description_.field_names.push_back(decl->name);
    } else {
        auto type = std::dynamic_pointer_cast<SimpleType>(decl->type);
        if (!type) {
            throw std::runtime_error("Arrays not supported yet");
        }
        if (type->name == "int") {
            current_layer_->DeclareSymbol(decl->name, std::make_shared<IntegerType>());
        } else {
            if (classes_descriptions_.count(type->name) == 0) {
                throw std::runtime_error("Unknown type: " + type->name);
            }
            current_layer_->DeclareSymbol(decl->name,
                 std::make_shared<ClassType>(classes_descriptions_[type->name].field_names, type->name));
        }
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<DeclarationList> decls) {
    for (auto decl : decls->declarations) {
        decl->Accept(shared_from_this());
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<Allocation> alloc) {
    // Not supported
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ArrayAllocation> alloc) {
    // Not supported
}

void SymbolTreeBuilder::Visit(std::shared_ptr<PlusExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<MinusExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<MulExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<DivideExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}


void SymbolTreeBuilder::Visit(std::shared_ptr<RemainderExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<CallExpr> expr) {
    expr->call->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<GreaterExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<GreaterEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<LessExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<LessEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<EqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<NotEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<TrueExpr> expr) {
}

void SymbolTreeBuilder::Visit(std::shared_ptr<FalseExpr> expr) {
}

void SymbolTreeBuilder::Visit(std::shared_ptr<NumberExpr> expr) {
}

void SymbolTreeBuilder::Visit(std::shared_ptr<LengthExpr> expr) {
    // Not supported
}

void SymbolTreeBuilder::Visit(std::shared_ptr<AndExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<OrExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    expr->rhs->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<NotExpr> expr) {
    expr->expr->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<LValueExpr> expr) {
    expr->value->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ThisExpr> expr) {
}

void SymbolTreeBuilder::Visit(std::shared_ptr<Variable> lvalue) {
    if (std::dynamic_pointer_cast<FunctionType>(current_layer_->GetValue(lvalue->name))) {
        throw std::runtime_error("Function must be called");
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ArrayElement> lvalue) {
    // Not supported
}

void SymbolTreeBuilder::Visit(std::shared_ptr<Program> prg) {
    root_ = std::make_shared<SymbolLayer>();
    current_layer_ = root_;

    // At first, create class descriptions
    mode_ = FIRST_PASS;
    prg->other_classes->Accept(shared_from_this());

    // Build tree
    mode_ = SECOND_PASS;
    prg->main_class->Accept(shared_from_this());
    prg->other_classes->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<Assert> stmt) {
}

void SymbolTreeBuilder::Visit(std::shared_ptr<Assignment> stmt) {
    stmt->lvalue->Accept(shared_from_this());
    stmt->rvalue->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<CallStatement> stmt) {
    stmt->call->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ComplexStatement> stmt) {
    if (mode_ == SECOND_PASS) {
        current_layer_ = std::make_shared<SymbolLayer>(current_layer_);
        current_layer_->GetParent()->AddChild(current_layer_);
        stmt->statement_list->Accept(shared_from_this());
        current_layer_ = current_layer_->GetParent();
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ConditionalStatement> stmt) {
    stmt->condition->Accept(shared_from_this());
    stmt->if_branch->Accept(shared_from_this());
    stmt->else_branch->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<WhileStatement> stmt) {
    stmt->condition->Accept(shared_from_this());
    stmt->statement->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<PrintStatement> stmt) {
    stmt->value->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ReturnStatement> stmt) {
    stmt->value->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<VariableStatement> stmt) {
    stmt->variable->Accept(shared_from_this());
}

void SymbolTreeBuilder::Visit(std::shared_ptr<StatementList> list) {
    for (auto stmt : list->statements) {
        stmt->Accept(shared_from_this());
    }
}

void SymbolTreeBuilder::Visit(std::shared_ptr<SimpleType> type) {
    /// Do nothing
}

void SymbolTreeBuilder::Visit(std::shared_ptr<ArrayType> type) {
    /// Do nothing
}

SymbolTree SymbolTreeBuilder::GetSymbolTree() const {
    return SymbolTree(root_);
}
