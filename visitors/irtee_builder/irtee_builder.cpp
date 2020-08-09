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

IRTreeBuilder::IRTreeBuilder(const SymbolTree& symbol_tree)
    : symbol_tree_(symbol_tree), storage_(ClassStorage::GetInstance()) {
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
    frame_ = std::make_shared<IR::Frame>("@mail_class", "main");
    frame_->BeginScope();

    current_layer_ = symbol_tree_.GetRoot()->GetChild("main");
    layer_number_.push(0);

    decl->main_statements->Accept(shared_from_this());
    ir_node_ = std::make_shared<IR::StatementWrapper>(
        std::make_shared<IR::SequenceStatement>(
            std::make_shared<IR::LabelStatement>(IR::Label("main")),
            ir_node_->ToStatement()
        )
    );

    layer_number_.pop();
    frame_->EndScope();
    frame_ = nullptr;
}

void IRTreeBuilder::Visit(std::shared_ptr<MethodDecl> decl) {
    current_layer_ = current_layer_->GetChild(decl->name);
    layer_number_.push(0);

    frame_ = std::make_shared<IR::Frame>(class_name_, decl->name);
    frame_->BeginScope();

    decl->statements->Accept(shared_from_this());

    ir_node_ = std::make_shared<IR::StatementWrapper>(
        std::make_shared<IR::SequenceStatement>(
            std::make_shared<IR::LabelStatement>(IR::Label(decl->name + "@" + class_name_)),
            ir_node_->ToStatement()
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
}

void IRTreeBuilder::Visit(std::shared_ptr<PlusExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<MinusExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<MulExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<DivideExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<RemainderExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<CallExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<GreaterExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<GreaterEqualExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<LessExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<LessEqualExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<EqualExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<NotEqualExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<TrueExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<FalseExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<NumberExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<LengthExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<AndExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<OrExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<NotExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<LValueExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<ThisExpr> expr) {
}

void IRTreeBuilder::Visit(std::shared_ptr<Variable> lvalue) {
}

void IRTreeBuilder::Visit(std::shared_ptr<ArrayElement> lvalue) {
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
}

void IRTreeBuilder::Visit(std::shared_ptr<Assignment> stmt) {
}

void IRTreeBuilder::Visit(std::shared_ptr<CallStatement> stmt) {
}

void IRTreeBuilder::Visit(std::shared_ptr<ComplexStatement> stmt) {
}

void IRTreeBuilder::Visit(std::shared_ptr<ConditionalStatement> stmt) {
}

void IRTreeBuilder::Visit(std::shared_ptr<WhileStatement> stmt) {
}

void IRTreeBuilder::Visit(std::shared_ptr<PrintStatement> stmt) {
}

void IRTreeBuilder::Visit(std::shared_ptr<ReturnStatement> stmt) {
}

void IRTreeBuilder::Visit(std::shared_ptr<VariableStatement> stmt) {
}

void IRTreeBuilder::Visit(std::shared_ptr<StatementList> list) {
}

void IRTreeBuilder::Visit(std::shared_ptr<SimpleType> type) {
}

void IRTreeBuilder::Visit(std::shared_ptr<ArrayType> type) {
}
