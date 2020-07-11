#ifndef CALL_STATEMENT_H
#define CALL_STATEMENT_H

#include <program/call/call.h>
#include <program/statement/statement.h>

class CallStatement : public Statement, public std::enable_shared_from_this<CallStatement> {
public:
    CallStatement(std::shared_ptr<MethodCall> call);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<MethodCall> call;
};

#endif