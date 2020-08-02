#ifndef TYPES
#define TYPES

#include <program/node.h>

class GrammaticsType : public Node {
public:
    virtual std::string GetTypeName() = 0;
};

class SimpleType : public GrammaticsType, public std::enable_shared_from_this<SimpleType> {
public:
    SimpleType(const std::string& name);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::string GetTypeName() override;

    std::string name;
};

class ArrayType : public GrammaticsType, public std::enable_shared_from_this<ArrayType> {
public:
    ArrayType(std::shared_ptr<SimpleType> element_type);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::string GetTypeName() override;

    std::shared_ptr<SimpleType> element_type;
};

#endif