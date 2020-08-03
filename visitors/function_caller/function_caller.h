#ifndef FUNCTION_CALLER_H
#define FUNCTION_CALLER_H

#include <class_storage/class_storage.h>
#include <program/visitor.h>
#include <symbol_tree/symbol_tree.h>
#include <function_mechanism/frame/frame.h>
#include <function_mechanism/index_table/index_table.h>

class FunctionCaller : public Visitor, public std::enable_shared_from_this<FunctionCaller> {
public:
    FunctionCaller(
        const SymbolTree& symbol_tree,
        std::shared_ptr<ClassType> this_pointer,
        const std::string& method_name,
        const std::vector<std::shared_ptr<Type>>& parameters = {},
        Frame* parent_frame = nullptr
    );

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
    std::shared_ptr<Type> last_value_;
    bool returned_ = false;

    SymbolTree symbol_tree_;
    std::shared_ptr<SymbolLayer> current_layer_;

    std::stack<size_t> layer_number_;

    Frame frame_;
    IndexTable variable_indices_;

    std::shared_ptr<ClassType> this_pointer_;
    std::string method_name_;
    std::shared_ptr<ClassEntry> class_descr_;
    MethodEntry method_descr_;

    ClassStorage& storage_;
};


#endif
