#ifndef EXPR_CALL
#define EXPR_CALL

#include <program/call/call.h>
#include <program/expression/expression.h>

class CallExpr : public Expression, public std::enable_shared_from_this<CallExpr> {
public:
    CallExpr(std::shared_ptr<MethodCall> call);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<MethodCall> call;
};

#endif