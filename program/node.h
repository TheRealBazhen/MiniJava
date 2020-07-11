#ifndef NODE_H
#define NODE_H

#include <memory>

class Visitor;

class Node {
public:
    virtual ~Node() = default;

    virtual void Accept(std::shared_ptr<Visitor> visitor) = 0;
};

#endif