#ifndef SYMBOL_TREE_H
#define SYMBOL_TREE_H

#include <symbol_tree/symbol_layer.h>

#include <iostream>
#include <memory>

class SymbolTree {
public:
    explicit SymbolTree(std::shared_ptr<SymbolLayer> root);

    std::shared_ptr<SymbolLayer> GetRoot() const;

private:
    std::shared_ptr<SymbolLayer> root_; 
};

std::ostream& operator<<(std::ostream& out, const SymbolTree& tree);

#endif