#include <class_storage/class_storage.h>
#include <types/class.h>
#include <types/integer.h>

#include <stdexcept>

ClassStorage ClassStorage::instance_;

ClassEntry::ClassEntry(const std::string& name) : name_(name) {
}

std::string ClassEntry::GetName() const {
    return name_;
}

void ClassEntry::AddField(const std::string& type, const std::string& name) {
    if (field_types_.count(name) > 0) {
        throw std::runtime_error("Field " + name + " of class "  + name_ + " already declared");
    }

    fields_.emplace_back(type, name);
    field_types_[name] = type;
}

void ClassEntry::AddMethod(const MethodEntry& method) {
    if (methods_map_.count(method.name) > 0) {
        throw std::runtime_error("Method " + method.name + " of class " + name_ + "already declared");
    }
    methods_.push_back(method);
    methods_map_[method.name] = method;
}

std::vector<std::pair<std::string, std::string>> ClassEntry::GetFieldList() const {
    return fields_;
}

std::vector<MethodEntry> ClassEntry::GetMethodList() const {
    return methods_;
}

std::string ClassEntry::GetFieldType(const std::string& name) const {
    return field_types_.at(name);
}

MethodEntry ClassEntry::GetMethod(const std::string& name) const {
    return methods_map_.at(name);
}

ClassStorage& ClassStorage::GetInstance() {
    return instance_;
}

std::shared_ptr<ClassEntry> ClassStorage::AddClassEntry(const std::string& name) {
    if (class_map_.count(name) > 0) {
        throw std::runtime_error("Class " + name + " already declared");
    }
    class_map_[name] = std::make_shared<ClassEntry>(name);
    class_list_.push_back(class_map_[name]);
    return class_map_[name];
}

std::shared_ptr<ClassEntry> ClassStorage::GetClassEntry(const std::string& name) {
    if (class_map_.count(name) == 0) {
        throw std::runtime_error("Class " + name + " not declared");
    }
    return class_map_[name];
}

std::vector<std::shared_ptr<ClassEntry>> ClassStorage::GetClassList() {
    return class_list_;
}

std::shared_ptr<Type> ClassStorage::MakeValue(const std::string& class_name) {
    if (class_name == "int" || class_name == "boolean") {
        return std::make_shared<IntegerType>();
    } else {
        auto class_descr = GetClassEntry(class_name);
        auto fields_descr = class_descr->GetFieldList();
        std::vector<std::shared_ptr<Type>> values;
        std::vector<std::string> names;
        values.reserve(fields_descr.size());
        names.reserve(fields_descr.size());
        for (const auto& [type, name] : fields_descr) {
            names.push_back(name);
            values.push_back(MakeValue(type));
        }
        return std::make_shared<ClassType>(names, values, class_name);
    }
}

size_t ClassStorage::GetTypeSize(const std::string& type_name) {
    size_t word_size = 4;
    if (type_name == "int" || type_name == "boolean") {
        return word_size;
    } else {
        auto class_descr = GetClassEntry(type_name);
        auto fields_descr = class_descr->GetFieldList();
        size_t res = 0;
        for (const auto& [type, name] : fields_descr) {
            res += GetTypeSize(type);
        }
        return res;
    }
}
