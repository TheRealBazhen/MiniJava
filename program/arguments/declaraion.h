#ifndef ARG_DECLARATION
#define ARG_DECLARATION

#include <program/types/types.h>

#include <vector>

class ArgumentDecl : public Node, public std::enable_shared_from_this<ArgumentDecl> {
public:
    ArgumentDecl(std::shared_ptr<Type> type, const std::string& name);
    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Type> type;
    std::string name;
};

class ArgumentDeclList : public Node, public std::enable_shared_from_this<ArgumentDeclList> {
public:
    void AddArgumentDecl(std::shared_ptr<ArgumentDecl> argument);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::vector<std::shared_ptr<ArgumentDecl>> arguments;
};

#endif