#ifndef IR_NODE_H
#define IR_NODE_H

#include <memory>

namespace IR {
class Visitor;

class Node {
public:
    virtual ~Node() = default;

    virtual void Accept(std::shared_ptr<Visitor> visitor) = 0;
};
}

#endif
