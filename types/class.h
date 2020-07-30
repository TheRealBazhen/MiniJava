#ifndef CLASS_H
#define CLASS_H

#include <types/type.h>

#include <memory>
#include <vector>
#include <unordered_map>

class ClassType : public Type {
public:
    ClassType(const std::vector<std::string>& field_names, const std::string& class_name);

    int GetIntValue() override;

    std::string GetClassName();
private:
    std::string name_;
    std::vector<std::shared_ptr<Type>> fields_;
    std::vector<std::string> field_names_;
    std::unordered_map<std::string, size_t> fileds_offsets_;
};

#endif