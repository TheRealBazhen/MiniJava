#include <types/class.h>
#include <types/function.h>
#include <types/integer.h>

ClassType::ClassType(
        const std::vector<std::string>& field_names,
        const std::vector<std::shared_ptr<Type>>& values,
        const std::string& class_name
) : fields_(values), field_names_(field_names), name_(class_name) {
    for (size_t i = 0; i < field_names.size(); ++i) {
        fileds_offsets_[field_names_[i]] = i;
    }
}

int ClassType::GetIntValue() {
    throw std::runtime_error("Not able to cast value of '" + name_ + "' to integer");
}

std::string ClassType::GetClassName() {
    return name_;
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

IntegerType::IntegerType(int value) : value_(value) {
}

int IntegerType::GetIntValue() {
    return value_;
}
