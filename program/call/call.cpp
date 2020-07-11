#include <program/visitor.h>

MethodCall::MethodCall(
    std::shared_ptr<CallableExpr> object,
    const std::string& method_name,
    std::shared_ptr<ArgumentValues> values)
    : object(object), method_name(method_name), values(values) {
}

MethodCall::MethodCall(
    std::shared_ptr<CallableExpr> object,
    const std::string& method_name)
    : object(object), method_name(method_name) {
}

void MethodCall::Accept(std::shared_ptr<Visitor> visitor) {
    visitor->Visit(shared_from_this());
}
