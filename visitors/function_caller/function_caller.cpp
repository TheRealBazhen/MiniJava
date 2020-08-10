#include <visitors/function_caller/function_caller.h>

FunctionCaller::FunctionCaller(
    const SymbolTree& symbol_tree,
    std::shared_ptr<ClassType> this_pointer,
    const std::string& method_name,
    const std::vector<std::shared_ptr<Type>>& parameters,
    Frame* parent_frame
) : symbol_tree_(symbol_tree), frame_(this_pointer->GetFieldValues(), parameters, parent_frame),
    storage_(ClassStorage::GetInstance()), this_pointer_(this_pointer), method_name_(method_name) {
}

void FunctionCaller::Visit(std::shared_ptr<ArgumentDecl> arg) {
    // Ignore
}

void FunctionCaller::Visit(std::shared_ptr<ArgumentDeclList> args) {
    // Ignore
}

void FunctionCaller::Visit(std::shared_ptr<ArgumentValues> vals) {
    // Ignore
}

void FunctionCaller::Visit(std::shared_ptr<MethodCall> call) {
    // Evaluate call object
    call->object->Accept(shared_from_this());
    auto call_object = std::dynamic_pointer_cast<ClassType>(last_value_);
    if (!call_object) {
        throw std::runtime_error("Method can be called only from class instance");
    }

    auto call_class_descr = storage_.GetClassEntry(call_object->GetTypeName());
    auto call_method_descr = call_class_descr->GetMethod(call->method_name);

    // Validate arguments number
    if (call->values->values.size() != call_method_descr.arguments.size()) {
        throw std::runtime_error(
            call_object->GetTypeName() + "." + call_method_descr.name + ": number of arguments mismatch, expected " +
            std::to_string(call_method_descr.arguments.size()) + ", got " + std::to_string(call->values->values.size())
        );
    }

    // Evaluate arguments and validate types
    std::vector<std::shared_ptr<Type>> arg_values;
    arg_values.reserve(call->values->values.size());
    for (size_t i = 0, size = call->values->values.size(); i < size; ++i) {
        call->values->values[i]->Accept(shared_from_this());
        if (last_value_->GetTypeName() != call_method_descr.arguments[i].first) {
            throw std::runtime_error(
                call_object->GetTypeName() + "." + call_method_descr.name + ": type mismatch of argument " +
                std::to_string(i + 1) + ", expected " + call_method_descr.arguments[i].first + ", got " +
                last_value_->GetTypeName()
            );
        }
        arg_values.push_back(last_value_);
    }

    auto caller = std::make_shared<FunctionCaller>(
        symbol_tree_,
        call_object, // this pointer
        call_method_descr.name,
        arg_values,
        &frame_
    );
    call_method_descr.code_location->Accept(caller);

    last_value_ = frame_.GetReturnValue();
}

void FunctionCaller::Visit(std::shared_ptr<ClassDeclaration> decl) {
    // Ignore
}

void FunctionCaller::Visit(std::shared_ptr<ClassDeclList> decls) {
    // Ignore
}

void FunctionCaller::Visit(std::shared_ptr<MainClassDecl> decl) {
    // Initialize index table
    variable_indices_.BeginScope();
    frame_.AllocScope();
    class_descr_ = storage_.GetClassEntry(this_pointer_->GetTypeName());
    method_descr_ = class_descr_->GetMethod(method_name_);

    current_layer_ = symbol_tree_.GetRoot()->GetChild("main");
    layer_number_.push(0);

    decl->main_statements->Accept(shared_from_this());

    variable_indices_.EndScope();
    frame_.DeallocScope();
}

void FunctionCaller::Visit(std::shared_ptr<MethodDecl> decl) {
    // Initialize index table
    variable_indices_.BeginScope();
    frame_.AllocScope();
    size_t index = 0;
    // 1. Fields
    class_descr_ = storage_.GetClassEntry(this_pointer_->GetTypeName());
    auto fields = class_descr_->GetFieldList();
    for (const auto& [type, name] : fields) {
        variable_indices_.CreateVariable(name);
        variable_indices_.PutIndex(name, index);
        ++index;
    }
    // 2. Parameters
    method_descr_ = class_descr_->GetMethod(method_name_);
    for (const auto& [type, name] : method_descr_.arguments) {
        variable_indices_.CreateVariable(name);
        variable_indices_.PutIndex(name, index);
        ++index;
    }

    current_layer_ = symbol_tree_.GetRoot()->GetChild(this_pointer_->GetTypeName())->GetChild(method_name_);
    layer_number_.push(0);

    decl->statements->Accept(shared_from_this());

    variable_indices_.EndScope();
    frame_.DeallocScope();

    if (!returned_ && method_descr_.return_type != "void") {
        throw std::runtime_error(
            "Method '" + method_name_ + "' of class '" + class_descr_->GetName() +
            "' is not marked void, but did not return any value"
        );
    }

    // Set fields back
    std::vector<std::shared_ptr<Type>> new_fields;
    new_fields.reserve(fields.size());
    for (size_t i = 0; i < fields.size(); ++i) {
        new_fields.push_back(frame_.GetValue(i));
    }
    this_pointer_->SetFieldValues(new_fields);
}

void FunctionCaller::Visit(std::shared_ptr<VariableDecl> decl) {
    size_t index = frame_.AllocVariable();
    variable_indices_.CreateVariable(decl->name);
    variable_indices_.PutIndex(decl->name, index);
}

void FunctionCaller::Visit(std::shared_ptr<DeclarationList> decls) {
    // Ignore
}

void FunctionCaller::Visit(std::shared_ptr<Allocation> alloc) {
    last_value_ = storage_.MakeValue(alloc->type->GetTypeName());
}

void FunctionCaller::Visit(std::shared_ptr<ArrayAllocation> alloc) {
    throw std::runtime_error("Arrays not supported yet");
}

void FunctionCaller::Visit(std::shared_ptr<PlusExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs + rhs);
}

void FunctionCaller::Visit(std::shared_ptr<MinusExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs - rhs);
}

void FunctionCaller::Visit(std::shared_ptr<MulExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs * rhs);
}

void FunctionCaller::Visit(std::shared_ptr<DivideExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs / rhs);
}

void FunctionCaller::Visit(std::shared_ptr<RemainderExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs % rhs);
}

void FunctionCaller::Visit(std::shared_ptr<CallExpr> expr) {
    expr->call->Accept(shared_from_this());
}

void FunctionCaller::Visit(std::shared_ptr<GreaterExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs > rhs);
}

void FunctionCaller::Visit(std::shared_ptr<GreaterEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs >= rhs);
}

void FunctionCaller::Visit(std::shared_ptr<LessExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs < rhs);
}

void FunctionCaller::Visit(std::shared_ptr<LessEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs <= rhs);
}

void FunctionCaller::Visit(std::shared_ptr<EqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs == rhs);
}

void FunctionCaller::Visit(std::shared_ptr<NotEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs != rhs);
}

void FunctionCaller::Visit(std::shared_ptr<TrueExpr> expr) {
    last_value_ = std::make_shared<IntegerType>(1);
}

void FunctionCaller::Visit(std::shared_ptr<FalseExpr> expr) {
    last_value_ = std::make_shared<IntegerType>(0);
}

void FunctionCaller::Visit(std::shared_ptr<NumberExpr> expr) {
    last_value_ = std::make_shared<IntegerType>(expr->number);
}

void FunctionCaller::Visit(std::shared_ptr<LengthExpr> expr) {
    throw std::runtime_error("Arrays not supported yet");
}

void FunctionCaller::Visit(std::shared_ptr<AndExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs && rhs);
}

void FunctionCaller::Visit(std::shared_ptr<OrExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    int lhs = last_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    int rhs = last_value_->GetIntValue();
    last_value_ = std::make_shared<IntegerType>(lhs || rhs);
}

void FunctionCaller::Visit(std::shared_ptr<NotExpr> expr) {
    expr->expr->Accept(shared_from_this());
    last_value_ = std::make_shared<IntegerType>(!last_value_->GetIntValue());
}

void FunctionCaller::Visit(std::shared_ptr<LValueExpr> expr) {
    expr->value->Accept(shared_from_this());
}

void FunctionCaller::Visit(std::shared_ptr<ThisExpr> expr) {
    last_value_ = this_pointer_;
}

void FunctionCaller::Visit(std::shared_ptr<Variable> lvalue) {
    size_t index = variable_indices_.GetIndex(lvalue->name);
    last_value_ = frame_.GetValue(index);
    if (!last_value_) {
        throw std::runtime_error("Use of uninitialized variable: " + lvalue->name);
    }
}

void FunctionCaller::Visit(std::shared_ptr<ArrayElement> lvalue) {
    throw std::runtime_error("Arrays not supported yet");
}

void FunctionCaller::Visit(std::shared_ptr<Program> prg) {
    // Ignore
}

void FunctionCaller::Visit(std::shared_ptr<Assert> stmt) {
    stmt->condition->Accept(shared_from_this());
    if (last_value_->GetIntValue() == 0) {
        throw std::runtime_error("Assertion failed");
    }
}

void FunctionCaller::Visit(std::shared_ptr<Assignment> stmt) {
    auto var = std::dynamic_pointer_cast<Variable>(stmt->lvalue);
    if (!var) {
        throw std::runtime_error("Arrays not supported yet");
    }

    stmt->rvalue->Accept(shared_from_this());

    // Check type match
    auto expected_type = current_layer_->GetTypeName(var->name);
    auto real_type = last_value_->GetTypeName();
    if (expected_type != real_type) {
        throw std::runtime_error(
            "Assignment types mismatch: expected " + expected_type + ", got " + real_type
        );
    }
    size_t index = variable_indices_.GetIndex(var->name);
    frame_.SetValue(index, last_value_);
}

void FunctionCaller::Visit(std::shared_ptr<CallStatement> stmt) {
    stmt->call->Accept(shared_from_this());
}

void FunctionCaller::Visit(std::shared_ptr<ComplexStatement> stmt) {
    current_layer_ = current_layer_->GetChild(layer_number_.top());
    layer_number_.push(0);

    frame_.AllocScope();
    variable_indices_.BeginScope();

    stmt->statement_list->Accept(shared_from_this());

    frame_.DeallocScope();
    variable_indices_.EndScope();

    layer_number_.pop();
    ++layer_number_.top();
    current_layer_ = current_layer_->GetParent();
}

void FunctionCaller::Visit(std::shared_ptr<ConditionalStatement> stmt) {
    stmt->condition->Accept(shared_from_this());
    int cond = last_value_->GetIntValue();

    if (cond) {
        stmt->if_branch->Accept(shared_from_this());
        // Skip else scope if necessary
        if (stmt->else_branch && std::dynamic_pointer_cast<ComplexStatement>(stmt->else_branch)) {
            ++layer_number_.top();
        }
    } else {
        // Skip if scope if necessary
        if (std::dynamic_pointer_cast<ComplexStatement>(stmt->if_branch)) {
            ++layer_number_.top();
        }
        if (stmt->else_branch) {
            stmt->else_branch->Accept(shared_from_this());
        }
    }
}

void FunctionCaller::Visit(std::shared_ptr<WhileStatement> stmt) {
    int layer_number = layer_number_.top();

    while (true) {
        stmt->condition->Accept(shared_from_this());
        if (!last_value_->GetIntValue()) {
            break;
        }
        layer_number_.top() = layer_number;
        stmt->statement->Accept(shared_from_this());
        if (returned_) {
            break;
        }
    }
}

void FunctionCaller::Visit(std::shared_ptr<PrintStatement> stmt) {
    stmt->value->Accept(shared_from_this());
    std::cout << last_value_->GetIntValue() << std::endl;
}

void FunctionCaller::Visit(std::shared_ptr<ReturnStatement> stmt) {
    stmt->value->Accept(shared_from_this());
    if (method_descr_.return_type != last_value_->GetTypeName()) {
        throw std::runtime_error(
            class_descr_->GetName() + "." + method_name_ + ": return type mismatch, expected " +
            method_descr_.return_type + ", got " + last_value_->GetTypeName()
        );
    }
    frame_.SetParentReturnValue(last_value_);
    returned_ = true;
}

void FunctionCaller::Visit(std::shared_ptr<VariableStatement> stmt) {
    stmt->variable->Accept(shared_from_this());
}

void FunctionCaller::Visit(std::shared_ptr<StatementList> list) {
    for (auto stmt : list->statements) {
        stmt->Accept(shared_from_this());
        if (returned_) {
            break;
        }
    }
}

void FunctionCaller::Visit(std::shared_ptr<SimpleType> type) {
    // Ignore
}

void FunctionCaller::Visit(std::shared_ptr<ArrayType> type) {
    // Ignore
}
