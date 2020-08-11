#include <visitors/irtee_builder/irtee_builder.h>

#include <irtree/nodes/expression/binary_operation.h>
#include <irtree/nodes/expression/call.h>
#include <irtree/nodes/expression/constant.h>
#include <irtree/nodes/expression/eseq.h>
#include <irtree/nodes/expression/memory_access.h>
#include <irtree/nodes/expression/name.h>
#include <irtree/nodes/expression/temporary.h>
#include <irtree/nodes/statement/conditional_jump.h>
#include <irtree/nodes/statement/expression.h>
#include <irtree/nodes/statement/jump.h>
#include <irtree/nodes/statement/label.h>
#include <irtree/nodes/statement/move.h>
#include <irtree/nodes/statement/sequence.h>
#include <irtree/wrapper/conditional/comparisons/comparisons.h>
#include <irtree/wrapper/conditional/logic/logic.h>
#include <irtree/wrapper/expression/expression.h>
#include <irtree/wrapper/statement/statement.h>

#include <stdexcept>

IRTreeBuilder::IRTreeBuilder(const SymbolTree& symbol_tree)
    : symbol_tree_(symbol_tree), storage_(ClassStorage::GetInstance()) {
}

std::shared_ptr<IR::Statement> IRTreeBuilder::GetIRTree() {
    return ir_tree_;
}

void IRTreeBuilder::Visit(std::shared_ptr<ArgumentDecl> arg) {
    // Ignore
}

void IRTreeBuilder::Visit(std::shared_ptr<ArgumentDeclList> args) {
    // ignore
}

void IRTreeBuilder::Visit(std::shared_ptr<ArgumentValues> vals) {
    // Ignore
}

void IRTreeBuilder::Visit(std::shared_ptr<MethodCall> call) {
    auto arguments = std::make_shared<IR::ExpressionList>();

    call->object->Accept(shared_from_this());
    arguments->AddExpression(ir_node_->ToExpression());

    std::string call_class_name(node_value_->GetTypeName());
    std::string call_method_name(call->method_name);
    IR::Label method_label(call_method_name + "@" + call_class_name);

    // Validate arguments
    auto call_method_descr = storage_.GetClassEntry(call_class_name)->GetMethod(call_method_name);
    if (call_method_descr.arguments.size() != call->values->values.size()) {
        throw std::runtime_error(
            call_class_name + "::" + call_method_name + " call arguments number mismatch: expected " +
            std::to_string(call_method_descr.arguments.size()) + ", got " + std::to_string(call->values->values.size())
        );
    }

    for (size_t i = 0, size = call_method_descr.arguments.size(); i < size; ++i) {
        call->values->values[i]->Accept(shared_from_this());
        arguments->AddExpression(ir_node_->ToExpression());

        // Validate argument type
        if (node_value_->GetTypeName() != call_method_descr.arguments[i].first) {
            throw std::runtime_error(
                call_class_name + "::" + call_method_name + " call argument " + std::to_string(i + 1) +
                " type mismatch: expected " + std::to_string(call_method_descr.arguments.size()) +
                ", got " + std::to_string(call->values->values.size())
            );
        }
    }
    node_value_ = storage_.MakeValue(call_method_descr.return_type);
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(
        std::make_shared<IR::CallExpression>(
            std::make_shared<IR::NameExpression>(method_label),
            arguments
        )
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<ClassDeclaration> decl) {
    class_name_ = decl->name;
    current_layer_ = symbol_tree_.GetRoot()->GetChild(class_name_);

    decl->declarations->Accept(shared_from_this());
}

void IRTreeBuilder::Visit(std::shared_ptr<ClassDeclList> decls) {
    // Ignore
}

void IRTreeBuilder::Visit(std::shared_ptr<MainClassDecl> decl) {
    class_name_ = "@main_class";
    method_name_ = "main";

    frame_ = std::make_shared<IR::Frame>(class_name_, method_name_);
    frame_->BeginScope();

    current_layer_ = symbol_tree_.GetRoot()->GetChild(method_name_);
    layer_number_.push(0);

    decl->main_statements->Accept(shared_from_this());
    ir_node_ = std::make_shared<IR::StatementWrapper>(
        std::make_shared<IR::SequenceStatement>(
            std::make_shared<IR::LabelStatement>(IR::Label(method_name_)),
            std::make_shared<IR::SequenceStatement>(
                ir_node_->ToStatement(),
                std::make_shared<IR::JumpStatement>(frame_->GetReturnAddress()->ToExpression())
            )
        )
    );

    layer_number_.pop();
    frame_->EndScope();
    frame_ = nullptr;
}

void IRTreeBuilder::Visit(std::shared_ptr<MethodDecl> decl) {
    method_name_ = decl->name;

    current_layer_ = current_layer_->GetChild(decl->name);
    layer_number_.push(0);

    frame_ = std::make_shared<IR::Frame>(class_name_, decl->name);
    frame_->BeginScope();

    decl->statements->Accept(shared_from_this());

    ir_node_ = std::make_shared<IR::StatementWrapper>(
        std::make_shared<IR::SequenceStatement>(
            std::make_shared<IR::LabelStatement>(IR::Label(decl->name + "@" + class_name_)),
            std::make_shared<IR::SequenceStatement>(
                ir_node_->ToStatement(),
                std::make_shared<IR::JumpStatement>(frame_->GetReturnAddress()->ToExpression())
            )
        )
    );

    frame_->EndScope();
    frame_ = nullptr;

    layer_number_.pop();
    current_layer_ = current_layer_->GetParent();
}

void IRTreeBuilder::Visit(std::shared_ptr<VariableDecl> decl) {
    if (frame_) {
        frame_->AddVariable(decl->name);
    }
    ir_node_ = nullptr;
}

void IRTreeBuilder::Visit(std::shared_ptr<DeclarationList> decls) {
    std::shared_ptr<IR::Statement> stmt;
    for (auto decl : decls->declarations) {
        decl->Accept(shared_from_this());
        if (ir_node_) {
            if (stmt) {
                stmt = std::make_shared<IR::SequenceStatement>(stmt, ir_node_->ToStatement());
            } else {
                stmt = ir_node_->ToStatement();
            }
        }
    }
    ir_node_ = std::make_shared<IR::StatementWrapper>(stmt);
}

void IRTreeBuilder::Visit(std::shared_ptr<Allocation> alloc) {
    node_value_ = storage_.MakeValue(alloc->type->name);

    auto temp = std::make_shared<IR::TemporaryExpression>(IR::Temporary());
    size_t size = storage_.GetTypeSize(alloc->type->name);

    auto malloc_args = std::make_shared<IR::ExpressionList>();
    malloc_args->AddExpression(std::make_shared<IR::ConstantExpression>(size));

    auto memset_args = std::make_shared<IR::ExpressionList>();
    memset_args->AddExpression(temp);
    memset_args->AddExpression(std::make_shared<IR::ConstantExpression>(0));
    memset_args->AddExpression(std::make_shared<IR::ConstantExpression>(size));

    auto expr = std::make_shared<IR::ESEQExpression>(
        std::make_shared<IR::SequenceStatement>(
            std::make_shared<IR::MoveStatement>(
                temp,
                std::make_shared<IR::CallExpression>(
                    std::make_shared<IR::NameExpression>(IR::Label("malloc")),
                    malloc_args
                )
            ),
            std::make_shared<IR::ExpressionStatement>(
                std::make_shared<IR::CallExpression>(
                    std::make_shared<IR::NameExpression>(IR::Label("memset")),
                    memset_args
                )
            )
        ),
        temp
    );
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(expr);
}

void IRTreeBuilder::Visit(std::shared_ptr<ArrayAllocation> alloc) {
    /// Not supported
    ir_node_ = nullptr;
    node_value_ = nullptr;
}

void IRTreeBuilder::Visit(std::shared_ptr<PlusExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(
        std::make_shared<IR::BinaryOperationExpression>(
            IR::BinaryOperaionType::PLUS,
            lhs, rhs
        )
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<MinusExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(
        std::make_shared<IR::BinaryOperationExpression>(
            IR::BinaryOperaionType::MINUS,
            lhs, rhs
        )
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<MulExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(
        std::make_shared<IR::BinaryOperationExpression>(
            IR::BinaryOperaionType::MUL,
            lhs, rhs
        )
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<DivideExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(
        std::make_shared<IR::BinaryOperationExpression>(
            IR::BinaryOperaionType::DIV,
            lhs, rhs
        )
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<RemainderExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(
        std::make_shared<IR::BinaryOperationExpression>(
            IR::BinaryOperaionType::REM,
            lhs, rhs
        )
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<CallExpr> expr) {
    expr->call->Accept(shared_from_this());
}

void IRTreeBuilder::Visit(std::shared_ptr<GreaterExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ComparisonWrapper>(
        IR::ConditionalOperatorType::GT,
        lhs, rhs
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<GreaterEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ComparisonWrapper>(
        IR::ConditionalOperatorType::GE,
        lhs, rhs
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<LessExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ComparisonWrapper>(
        IR::ConditionalOperatorType::LT,
        lhs, rhs
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<LessEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ComparisonWrapper>(
        IR::ConditionalOperatorType::LE,
        lhs, rhs
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<EqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ComparisonWrapper>(
        IR::ConditionalOperatorType::EQ,
        lhs, rhs
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<NotEqualExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_->ToExpression();
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::ComparisonWrapper>(
        IR::ConditionalOperatorType::NE,
        lhs, rhs
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<TrueExpr> expr) {
    node_value_ = std::make_shared<IntegerType>();
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(std::make_shared<IR::ConstantExpression>(1));
}

void IRTreeBuilder::Visit(std::shared_ptr<FalseExpr> expr) {
    node_value_ = std::make_shared<IntegerType>();
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(std::make_shared<IR::ConstantExpression>(0));
}

void IRTreeBuilder::Visit(std::shared_ptr<NumberExpr> expr) {
    node_value_ = std::make_shared<IntegerType>();
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(std::make_shared<IR::ConstantExpression>(expr->number));
}

void IRTreeBuilder::Visit(std::shared_ptr<LengthExpr> expr) {
    /// Not supported
    node_value_ = nullptr;
    ir_node_ = nullptr;
}

void IRTreeBuilder::Visit(std::shared_ptr<AndExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_;
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_;
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::AndConditionalWrapper>(lhs, rhs);
}

void IRTreeBuilder::Visit(std::shared_ptr<OrExpr> expr) {
    expr->lhs->Accept(shared_from_this());
    auto lhs = ir_node_;
    node_value_->GetIntValue();
    expr->rhs->Accept(shared_from_this());
    auto rhs = ir_node_;
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::OrConditionalWrapper>(lhs, rhs);
}

void IRTreeBuilder::Visit(std::shared_ptr<NotExpr> expr) {
    expr->expr->Accept(shared_from_this());
    node_value_->GetIntValue();
    ir_node_ = std::make_shared<IR::NegateConditionalWrapper>(ir_node_);
}

void IRTreeBuilder::Visit(std::shared_ptr<LValueExpr> expr) {
    expr->value->Accept(shared_from_this());
}

void IRTreeBuilder::Visit(std::shared_ptr<ThisExpr> expr) {
    node_value_ = storage_.MakeValue(class_name_);
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(frame_->GetThisPointer()->ToExpression());
}

void IRTreeBuilder::Visit(std::shared_ptr<Variable> lvalue) {
    node_value_ = current_layer_->GetValue(lvalue->name);
    ir_node_ = std::make_shared<IR::ExpressionWrapper>(frame_->GetAddress(lvalue->name)->ToExpression());
}

void IRTreeBuilder::Visit(std::shared_ptr<ArrayElement> lvalue) {
    /// Not supported
}

void IRTreeBuilder::Visit(std::shared_ptr<Program> prg) {
    prg->main_class->Accept(shared_from_this());
    auto stmt = ir_node_->ToStatement();

    for (auto decl : prg->other_classes->declarations) {
        decl->Accept(shared_from_this());
        stmt = std::make_shared<IR::SequenceStatement>(stmt, ir_node_->ToStatement());
    }

    ir_tree_ = stmt;
}

void IRTreeBuilder::Visit(std::shared_ptr<Assert> stmt) {
    /// TODO: do something with print call
    stmt->condition->Accept(shared_from_this());
    node_value_->GetIntValue();

    IR::Label true_label;
    IR::Label false_label;

    ir_node_ = std::make_shared<IR::StatementWrapper>(
        std::make_shared<IR::SequenceStatement>(
            ir_node_->ToConditional(true_label, false_label),
            std::make_shared<IR::SequenceStatement>(
                std::make_shared<IR::LabelStatement>(false_label),
                std::make_shared<IR::SequenceStatement>(
                    std::make_shared<IR::ExpressionStatement>(
                        std::make_shared<IR::CallExpression>(
                            std::make_shared<IR::NameExpression>(IR::Label("print_assert_failed")),
                            std::make_shared<IR::ExpressionList>()
                        )
                    ),
                    std::make_shared<IR::SequenceStatement>(
                        std::make_shared<IR::ExpressionStatement>(
                            std::make_shared<IR::CallExpression>(
                                std::make_shared<IR::NameExpression>(IR::Label("abort")),
                                std::make_shared<IR::ExpressionList>()
                            )
                        ),
                        std::make_shared<IR::LabelStatement>(true_label)
                    )
                )
            )
        )
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<Assignment> stmt) {
    stmt->lvalue->Accept(shared_from_this());
    auto lvalue = ir_node_->ToExpression();
    auto lvalue_type = node_value_->GetTypeName();
    stmt->rvalue->Accept(shared_from_this());
    auto rvalue = ir_node_->ToExpression();
    auto rvalue_type = node_value_->GetTypeName();

    if (lvalue_type != rvalue_type) {
        throw std::runtime_error("Assignment type mismatch: expected " + lvalue_type + ", got " + rvalue_type);
    }
    ir_node_ = std::make_shared<IR::StatementWrapper>(std::make_shared<IR::MoveStatement>(lvalue, rvalue));
}

void IRTreeBuilder::Visit(std::shared_ptr<CallStatement> stmt) {
    stmt->call->Accept(shared_from_this());
}

void IRTreeBuilder::Visit(std::shared_ptr<ComplexStatement> stmt) {
    current_layer_ = current_layer_->GetChild(layer_number_.top());
    ++layer_number_.top();
    layer_number_.push(0);

    frame_->BeginScope();

    stmt->statement_list->Accept(shared_from_this());

    frame_->EndScope();

    layer_number_.pop();
    current_layer_ = current_layer_->GetParent();
}

void IRTreeBuilder::Visit(std::shared_ptr<ConditionalStatement> stmt) {
    stmt->condition->Accept(shared_from_this());
    node_value_->GetIntValue();
    auto condition = ir_node_;

    stmt->if_branch->Accept(shared_from_this());
    auto true_stmt = ir_node_->ToStatement();

    if (stmt->else_branch) {
        stmt->else_branch->Accept(shared_from_this());
        auto false_stmt = ir_node_->ToStatement();

        IR::Label true_label;
        IR::Label false_label;
        IR::Label join_label;

        ir_node_ = std::make_shared<IR::StatementWrapper>(
            std::make_shared<IR::SequenceStatement>(
                condition->ToConditional(true_label, false_label),
                std::make_shared<IR::SequenceStatement>(
                    std::make_shared<IR::LabelStatement>(false_label),
                    std::make_shared<IR::SequenceStatement>(
                        false_stmt,
                        std::make_shared<IR::SequenceStatement>(
                            std::make_shared<IR::JumpStatement>(std::make_shared<IR::NameExpression>(join_label)),
                            std::make_shared<IR::SequenceStatement>(
                                std::make_shared<IR::LabelStatement>(true_label),
                                std::make_shared<IR::SequenceStatement>(
                                    true_stmt,
                                    std::make_shared<IR::LabelStatement>(join_label)
                                )
                            )
                        )
                    )
                )
            )
        );
    } else {
        IR::Label true_label;
        IR::Label false_label;

        ir_node_ = std::make_shared<IR::StatementWrapper>(
            std::make_shared<IR::SequenceStatement>(
                condition->ToConditional(true_label, false_label),
                std::make_shared<IR::SequenceStatement>(
                    std::make_shared<IR::LabelStatement>(true_label),
                    std::make_shared<IR::SequenceStatement>(
                        true_stmt,
                        std::make_shared<IR::LabelStatement>(false_label)
                    )
                )
            )
        );
    }
}

void IRTreeBuilder::Visit(std::shared_ptr<WhileStatement> stmt) {
    stmt->condition->Accept(shared_from_this());
    node_value_->GetIntValue();
    auto condition = ir_node_;

    stmt->statement->Accept(shared_from_this());
    auto body = ir_node_->ToStatement();

    IR::Label cycle_start;
    IR::Label cycle_body;
    IR::Label cycle_end;

    ir_node_ = std::make_shared<IR::StatementWrapper>(
        std::make_shared<IR::SequenceStatement>(
            std::make_shared<IR::LabelStatement>(cycle_start),
            std::make_shared<IR::SequenceStatement>(
                condition->ToConditional(cycle_body, cycle_end),
                std::make_shared<IR::SequenceStatement>(
                    std::make_shared<IR::LabelStatement>(cycle_body),
                    std::make_shared<IR::SequenceStatement>(
                        body,
                        std::make_shared<IR::SequenceStatement>(
                            std::make_shared<IR::JumpStatement>(std::make_shared<IR::NameExpression>(cycle_start)),
                            std::make_shared<IR::LabelStatement>(cycle_end)
                        )
                    )
                )
            )
        )
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<PrintStatement> stmt) {
    stmt->value->Accept(shared_from_this());
    node_value_->GetIntValue();
    auto expr = ir_node_->ToExpression();

    auto print_args = std::make_shared<IR::ExpressionList>();
    print_args->AddExpression(expr);

    ir_node_ = std::make_shared<IR::ExpressionWrapper>(
        std::make_shared<IR::CallExpression>(
            std::make_shared<IR::NameExpression>(IR::Label("print_int")),
            print_args
        )
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<ReturnStatement> stmt) {
    auto method_descr = storage_.GetClassEntry(class_name_)->GetMethod(method_name_);

    stmt->value->Accept(shared_from_this());
    auto value = ir_node_->ToExpression();
    std::string value_type = node_value_->GetTypeName();
    if (value_type != method_descr.return_type) {
        throw std::runtime_error("Return types mismatch: expected " + method_descr.return_type + ", got " + value_type);
    }

    ir_node_ = std::make_shared<IR::StatementWrapper>(
        std::make_shared<IR::SequenceStatement>(
            std::make_shared<IR::MoveStatement>(
                std::make_shared<IR::MemoryAccessExpression>(frame_->GetReturnValueAddress()->ToExpression()),
                value
            ),
            std::make_shared<IR::JumpStatement>(frame_->GetReturnAddress()->ToExpression())
        )
    );
}

void IRTreeBuilder::Visit(std::shared_ptr<VariableStatement> stmt) {
    stmt->variable->Accept(shared_from_this());
}

void IRTreeBuilder::Visit(std::shared_ptr<StatementList> list) {
    std::shared_ptr<IR::Statement> result;

    for (auto stmt : list->statements) {
        stmt->Accept(shared_from_this());
        if (ir_node_) {
            if (result) {
                result = std::make_shared<IR::SequenceStatement>(result, ir_node_->ToStatement());
            } else {
                result = ir_node_->ToStatement();
            }
        }
    }
    ir_node_ = std::make_shared<IR::StatementWrapper>(result);
}

void IRTreeBuilder::Visit(std::shared_ptr<SimpleType> type) {
    /// Ignore
}

void IRTreeBuilder::Visit(std::shared_ptr<ArrayType> type) {
    /// Ignore
}
