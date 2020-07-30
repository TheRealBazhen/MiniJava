#ifndef TYPES
#define TYPES

#include <program/node.h>

class GrammaticsType : public Node {
};

class SimpleType : public GrammaticsType, public std::enable_shared_from_this<SimpleType> {
public:
    SimpleType(const std::string& name);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::string name;
};

class ArrayType : public GrammaticsType, public std::enable_shared_from_this<ArrayType> {
public:
    ArrayType(std::shared_ptr<SimpleType> element_type);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<SimpleType> element_type;
};

#endif