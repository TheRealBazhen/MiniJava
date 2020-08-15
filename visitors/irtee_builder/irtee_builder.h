#ifndef IRTEE_BUILDER_H
#define IRTEE_BUILDER_H

#include <class_storage/class_storage.h>
#include <irtree/function_mechanism/frame/frame.h>
#include <irtree/nodes/statement/statement.h>
#include <irtree/wrapper/wrapper.h>
#include <program/visitor.h>
#include <symbol_tree/symbol_tree.h>

#include <string>

class IRTreeBuilder : public Visitor, public std::enable_shared_from_this<IRTreeBuilder> {
public:
    IRTreeBuilder(const SymbolTree& symbol_tree);

    std::shared_ptr<IR::Statement> GetIRTree();

    void Visit(std::shared_ptr<ArgumentDecl> arg) override;
    void Visit(std::shared_ptr<ArgumentDeclList> args) override;
    void Visit(std::shared_ptr<ArgumentValues> vals) override;
    void Visit(std::shared_ptr<MethodCall> call) override;
    void Visit(std::shared_ptr<ClassDeclaration> decl) override;
    void Visit(std::shared_ptr<ClassDeclList> decls) override;
    void Visit(std::shared_ptr<MainClassDecl> decl) override;
    void Visit(std::shared_ptr<MethodDecl> decl) override;
    void Visit(std::shared_ptr<VariableDecl> decl) override;
    void Visit(std::shared_ptr<DeclarationList> decls) override;
    void Visit(std::shared_ptr<Allocation> alloc) override;
    void Visit(std::shared_ptr<ArrayAllocation> alloc) override;
    void Visit(std::shared_ptr<PlusExpr> expr) override;
    void Visit(std::shared_ptr<MinusExpr> expr) override;
    void Visit(std::shared_ptr<MulExpr> expr) override;
    void Visit(std::shared_ptr<DivideExpr> expr) override;
    void Visit(std::shared_ptr<RemainderExpr> expr) override;
    void Visit(std::shared_ptr<CallExpr> expr) override;
    void Visit(std::shared_ptr<GreaterExpr> expr) override;
    void Visit(std::shared_ptr<GreaterEqualExpr> expr) override;
    void Visit(std::shared_ptr<LessExpr> expr) override;
    void Visit(std::shared_ptr<LessEqualExpr> expr) override;
    void Visit(std::shared_ptr<EqualExpr> expr) override;
    void Visit(std::shared_ptr<NotEqualExpr> expr) override;
    void Visit(std::shared_ptr<TrueExpr> expr) override;
    void Visit(std::shared_ptr<FalseExpr> expr) override;
    void Visit(std::shared_ptr<NumberExpr> expr) override;
    void Visit(std::shared_ptr<LengthExpr> expr) override;
    void Visit(std::shared_ptr<AndExpr> expr) override;
    void Visit(std::shared_ptr<OrExpr> expr) override;
    void Visit(std::shared_ptr<NotExpr> expr) override;
    void Visit(std::shared_ptr<LValueExpr> expr) override;
    void Visit(std::shared_ptr<ThisExpr> expr) override;
    void Visit(std::shared_ptr<Variable> lvalue) override;
    void Visit(std::shared_ptr<ArrayElement> lvalue) override;
    void Visit(std::shared_ptr<Program> prg) override;
    void Visit(std::shared_ptr<Assert> stmt) override;
    void Visit(std::shared_ptr<Assignment> stmt) override;
    void Visit(std::shared_ptr<CallStatement> stmt) override;
    void Visit(std::shared_ptr<ComplexStatement> stmt) override;
    void Visit(std::shared_ptr<ConditionalStatement> stmt) override;
    void Visit(std::shared_ptr<WhileStatement> stmt) override;
    void Visit(std::shared_ptr<PrintStatement> stmt) override;
    void Visit(std::shared_ptr<ReturnStatement> stmt) override;
    void Visit(std::shared_ptr<VariableStatement> stmt) override;
    void Visit(std::shared_ptr<StatementList> list) override;
    void Visit(std::shared_ptr<SimpleType> type) override;
    void Visit(std::shared_ptr<ArrayType> type) override;

private:
    std::shared_ptr<Type> node_value_;
    std::shared_ptr<IR::SubtreeWrapper> ir_node_;

    std::shared_ptr<IR::Statement> ir_tree_;

    SymbolTree symbol_tree_;
    std::shared_ptr<SymbolLayer> current_layer_;

    std::stack<size_t> layer_number_;

    std::shared_ptr<IR::Frame> frame_;

    ClassStorage& storage_;

    std::string class_name_;
    std::string method_name_;

    bool returned_;
};

#endif
