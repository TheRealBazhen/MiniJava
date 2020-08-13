#include <symbol_tree/symbol_tree.h>

SymbolTree::SymbolTree(std::shared_ptr<SymbolLayer> root) : root_(root) {
}

std::shared_ptr<SymbolLayer> SymbolTree::GetRoot() const {
    return root_;
}

std::ostream& operator<<(std::ostream& out, const SymbolTree& tree) {
    tree.GetRoot()->Print(out, 0);
    return out;
}
