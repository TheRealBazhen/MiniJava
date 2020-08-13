#ifndef CALL
#define CALL

#include <program/arguments/values.h>
#include <program/expression/expression.h>

#include <string>

class MethodCall : public Node, public std::enable_shared_from_this<MethodCall> {
public:
    MethodCall(std::shared_ptr<Expression> object, const std::string& method_name, std::shared_ptr<ArgumentValues> values);
    MethodCall(std::shared_ptr<Expression> object, const std::string& method_name);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> object;
    std::string method_name;
    std::shared_ptr<ArgumentValues> values;
};

#endif