#ifndef NAME_H
#define NAME_H

#include <irtree/nodes/expression/expression.h>
#include <irtree/generators/label/label.h>

namespace IR {
class NameExpression : public Expression, public std::enable_shared_from_this<NameExpression> {
public:
    NameExpression(const Label& label);

    void Accept(std::shared_ptr<Visitor> visitor) override;

public:
    Label label;
};
}

#endif
