#include <types/class.h>
#include <types/function.h>
#include <types/integer.h>

#include <stdexcept>

ClassType::ClassType(
        const std::vector<std::string>& field_names,
        const std::vector<std::shared_ptr<Type>>& values,
        const std::string& class_name
) : fields_(values), field_names_(field_names), name_(class_name) {
    for (size_t i = 0; i < field_names.size(); ++i) {
        fields_offsets_[field_names_[i]] = i;
    }
}

int ClassType::GetIntValue() {
    throw std::runtime_error("Not able to cast value of '" + name_ + "' to integer");
}

std::string ClassType::GetTypeName() {
    return name_;
}

std::vector<std::shared_ptr<Type>> ClassType::GetFieldValues() {
    return fields_;
}

void ClassType::SetFieldValues(std::vector<std::shared_ptr<Type>>& values) {
    fields_ = values;
    /// TODO: Maybe typechecking, but who matters
}

FunctionType::FunctionType(std::shared_ptr<MethodDecl> declaration)
    : name_(declaration->name), body_(declaration) {
    for (auto arg: declaration->arguments->arguments) {
        argument_names_.push_back(arg->name);
        ///
        arguments_.push_back(std::make_shared<IntegerType>());
    }
}

int FunctionType::GetIntValue() {
    throw std::runtime_error("Not able to cast function to int");
}

std::string FunctionType::GetTypeName() {
    return "function_" + name_;
}

IntegerType::IntegerType(int value) : value_(value) {
}

int IntegerType::GetIntValue() {
    return value_;
}

std::string IntegerType::GetTypeName() {
    return "int";
}
