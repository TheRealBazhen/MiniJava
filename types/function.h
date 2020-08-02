#ifndef FUNCTION_H
#define FUNCTION_H

#include <program/declaration/method.h>
#include <types/type.h>

#include <string>
#include <vector>

class FunctionType : public Type {
public:
    FunctionType(std::shared_ptr<MethodDecl> declaration);

    int GetIntValue() override;

private:
    std::string name_;
    std::vector<std::shared_ptr<Type>> arguments_;
    std::vector<std::string> argument_names_;
    std::shared_ptr<MethodDecl> body_;
};

#endif