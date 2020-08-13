#ifndef CLASS_H
#define CLASS_H

#include <types/type.h>

#include <memory>
#include <vector>
#include <unordered_map>

class ClassType : public Type {
public:
    ClassType(
            const std::vector<std::string>& field_names,
            const std::vector<std::shared_ptr<Type>>& values,
            const std::string& class_name
    );

    int GetIntValue() override;

    std::string GetTypeName() override;

    std::vector<std::shared_ptr<Type>> GetFieldValues();
    void SetFieldValues(std::vector<std::shared_ptr<Type>>& values);

private:
    std::string name_;
    std::vector<std::shared_ptr<Type>> fields_;
    std::vector<std::string> field_names_;
    std::unordered_map<std::string, size_t> fields_offsets_;
};

#endif