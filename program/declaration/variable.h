#ifndef VARIABLE
#define VARIABLE

#include <program/declaration/declaration.h>
#include <program/types/types.h>

#include <string>

class VariableDecl : public Declaration, public std::enable_shared_from_this<VariableDecl> {
public:
    VariableDecl(std::shared_ptr<GrammaticsType> type, const std::string& name);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<GrammaticsType> type;
    std::string name;
};

#endif