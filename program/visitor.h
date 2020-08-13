#ifndef VISITOR_H
#define VISITOR_H

#include <program/arguments/declaraion.h>
#include <program/arguments/values.h>
#include <program/call/call.h>
#include <program/declaration/class.h>
#include <program/declaration/method.h>
#include <program/declaration/variable.h>
#include <program/expression/allocation.h>
#include <program/expression/ariphmetics.h>
#include <program/expression/call.h>
#include <program/expression/compare.h>
#include <program/expression/constants.h>
#include <program/expression/length.h>
#include <program/expression/logic.h>
#include <program/expression/lvalue.h>
#include <program/expression/this.h>
#include <program/lvalue/lvalue.h>
#include <program/program/program.h>
#include <program/statement/assert.h>
#include <program/statement/assingment.h>
#include <program/statement/call.h>
#include <program/statement/complex.h>
#include <program/statement/conditional.h>
#include <program/statement/print.h>
#include <program/statement/return.h>
#include <program/statement/variable.h>
#include <program/types/types.h>

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void Visit(std::shared_ptr<ArgumentDecl> arg) = 0;
    virtual void Visit(std::shared_ptr<ArgumentDeclList> args) = 0;
    virtual void Visit(std::shared_ptr<ArgumentValues> vals) = 0;
    virtual void Visit(std::shared_ptr<MethodCall> call) = 0;
    virtual void Visit(std::shared_ptr<ClassDeclaration> decl) = 0;
    virtual void Visit(std::shared_ptr<ClassDeclList> decls) = 0;
    virtual void Visit(std::shared_ptr<MainClassDecl> decl) = 0;
    virtual void Visit(std::shared_ptr<MethodDecl> decl) = 0;
    virtual void Visit(std::shared_ptr<VariableDecl> decl) = 0;
    virtual void Visit(std::shared_ptr<DeclarationList> decls) = 0;
    virtual void Visit(std::shared_ptr<Allocation> alloc) = 0;
    virtual void Visit(std::shared_ptr<ArrayAllocation> alloc) = 0;
    virtual void Visit(std::shared_ptr<PlusExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<MinusExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<MulExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<DivideExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<RemainderExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<CallExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<GreaterExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<GreaterEqualExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<LessExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<LessEqualExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<EqualExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<NotEqualExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<TrueExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<FalseExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<NumberExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<LengthExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<AndExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<OrExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<NotExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<LValueExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<ThisExpr> expr) = 0;
    virtual void Visit(std::shared_ptr<Variable> lvalue) = 0;
    virtual void Visit(std::shared_ptr<ArrayElement> lvalue) = 0;
    virtual void Visit(std::shared_ptr<Program> prg) = 0;
    virtual void Visit(std::shared_ptr<Assert> stmt) = 0;
    virtual void Visit(std::shared_ptr<Assignment> stmt) = 0;
    virtual void Visit(std::shared_ptr<CallStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<ComplexStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<ConditionalStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<WhileStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<PrintStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<ReturnStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<VariableStatement> stmt) = 0;
    virtual void Visit(std::shared_ptr<StatementList> list) = 0;
    virtual void Visit(std::shared_ptr<SimpleType> type) = 0;
    virtual void Visit(std::shared_ptr<ArrayType> type) = 0;
};

#endif
