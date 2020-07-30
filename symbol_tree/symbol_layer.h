#ifndef SYMBOL_LAYER_H
#define SYMBOL_LAYER_H

#include <types/class.h>
#include <types/function.h>
#include <types/integer.h>

#include <iostream>
#include <unordered_map>

class SymbolLayer : public std::enable_shared_from_this<SymbolLayer> {
public:
    SymbolLayer() = default;
    SymbolLayer(std::shared_ptr<SymbolLayer> parent);

    void AddChild(std::shared_ptr<SymbolLayer> child);
    void AddAccociation(std::shared_ptr<SymbolLayer> child, const std::string& name);

    std::shared_ptr<SymbolLayer> GetChild(int idx) const;
    std::shared_ptr<SymbolLayer> GetChild(const std::string& name) const;
    std::shared_ptr<SymbolLayer> GetParent() const;

    void DeclareSymbol(const std::string& name, std::shared_ptr<Type> value);
    void SetValue(const std::string& name, std::shared_ptr<Type> value);
    std::shared_ptr<Type> GetValue(const std::string& name);

    void Print(std::ostream& out, size_t offset) const;

private:
    // Hierarchy
    std::weak_ptr<SymbolLayer> parent_;
    std::vector<std::shared_ptr<SymbolLayer>> children_;
    std::unordered_map<std::string, std::shared_ptr<SymbolLayer>> named_children_;

    // Symbols
    std::unordered_map<std::string, std::shared_ptr<Type>> symbols_;
    std::unordered_map<std::string, size_t> symbol_offset_;
};

#endif