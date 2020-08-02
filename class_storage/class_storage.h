#ifndef CLASS_STORAGE_H
#define CLASS_STORAGE_H

#include <program/visitor.h>
#include <types/type.h>

#include <unordered_map>

struct MethodEntry {
    std::string return_type;
    std::string name;
    std::vector<std::pair<std::string, std::string>> arguments; // [type, name]
    std::shared_ptr<StatementList> code_location;
};

class ClassEntry {
public:
    ClassEntry(const std::string& name);

    std::string GetName() const;

    void AddField(const std::string& type, const std::string& name);
    void AddMethod(const MethodEntry& method);

    std::vector<std::pair<std::string, std::string>> GetFieldList() const;
    std::vector<MethodEntry> GetMethodList() const;

    std::string GetFieldType(const std::string& name) const;
    MethodEntry GetMethod(const std::string& name) const;

private:
    std::string name_;
    // [type, field_name]
    std::vector<std::pair<std::string, std::string>> fields_;
    std::vector<MethodEntry> methods_;

    // Short access
    std::unordered_map<std::string, std::string> field_types_;
    std::unordered_map<std::string, MethodEntry> methods_map_;
};

class ClassStorage {
public:
    ClassStorage() = default;
    ClassStorage(const ClassStorage&) = delete;
    ClassStorage(ClassStorage&&) = delete;
    ClassStorage& operator=(const ClassStorage&) = delete;
    ClassStorage& operator=(ClassStorage&&) = delete;

    static ClassStorage& GetInstance();

    std::shared_ptr<ClassEntry> AddClassEntry(const std::string& name);
    std::shared_ptr<ClassEntry> GetClassEntry(const std::string& name);

    std::vector<std::shared_ptr<ClassEntry>> GetClassList();

    std::shared_ptr<Type> MakeValue(const std::string& class_name);

private:
    static ClassStorage instance_;

    std::vector<std::shared_ptr<ClassEntry>> class_list_;
    std::unordered_map<std::string, std::shared_ptr<ClassEntry>> class_map_;
};

#endif
