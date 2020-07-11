#ifndef DECLARATION
#define DECLARATION

#include <program/node.h>

#include <vector>

class Declaration : public Node {
};

class DeclarationList : public Node, public std::enable_shared_from_this<DeclarationList> {
public:
    void AddDeclaration(std::shared_ptr<Declaration> declaration);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::vector<std::shared_ptr<Declaration>> declarations;
};

#endif