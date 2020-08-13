#include <symbol_tree/symbol_layer.h>

#include <stdexcept>

SymbolLayer::SymbolLayer(std::shared_ptr<SymbolLayer> parent)
    : parent_(parent) {
}

void SymbolLayer::AddChild(std::shared_ptr<SymbolLayer> child) {
    children_.push_back(child);
}

void SymbolLayer::AddAssociation(std::shared_ptr<SymbolLayer> child, const std::string& name) {
    if (named_children_.count(name) > 0) {
        throw std::runtime_error("Named child already exists");
    }
    named_children_.emplace(name, child);
}

std::shared_ptr<SymbolLayer> SymbolLayer::GetChild(int idx) const {
    return children_.at(idx);
}

std::shared_ptr<SymbolLayer> SymbolLayer::GetChild(const std::string& name) const {
    return named_children_.at(name);
}

std::shared_ptr<SymbolLayer> SymbolLayer::GetParent() const {
    return parent_.lock();
}

void SymbolLayer::DeclareSymbol(const std::string& name, std::shared_ptr<Type> value) {
    if (symbols_.count(name) > 0) {
        throw std::runtime_error(std::string("Symbol already defined in scope: ") + name);
    }
    symbols_.emplace(name, value);
    symbol_offset_.emplace(name, symbols_.size() - 1);
    symbol_types_.emplace(name, value->GetTypeName());
}

void SymbolLayer::SetValue(const std::string& name, std::shared_ptr<Type> value) {
    auto cur_layer = shared_from_this();
    while (cur_layer) {
        if (cur_layer->symbols_.count(name) > 0) {
            if (cur_layer->symbol_types_[name] != value->GetTypeName()) {
                throw std::runtime_error(
                        "Assignment of '" + value->GetTypeName() +
                        "' to '" + cur_layer->symbol_types_[name] + "'object"
                );
            }
            cur_layer->symbols_[name] = value;
            return;
        }
        cur_layer = cur_layer->GetParent();
    }
    throw std::runtime_error(std::string("Symbol not declared: ") + name);
}

std::shared_ptr<Type> SymbolLayer::GetValue(const std::string& name) {
    auto cur_layer = shared_from_this();
    while (cur_layer) {
        if (cur_layer->symbols_.count(name) > 0) {
            return cur_layer->symbols_.at(name);
        }
        cur_layer = cur_layer->GetParent();
    }
    throw std::runtime_error(std::string("Symbol not declared: ") + name);
}

std::string SymbolLayer::GetTypeName(const std::string& symbol) {
    auto cur_layer = shared_from_this();
    while (cur_layer) {
        if (cur_layer->symbols_.count(symbol) > 0) {
            return cur_layer->symbol_types_.at(symbol);
        }
        cur_layer = cur_layer->GetParent();
    }
    throw std::runtime_error(std::string("Symbol not declared: ") + symbol);
}

void SymbolLayer::Print(std::ostream& out, size_t offset) const {
    out << std::string(offset, ' ') << "New layer\n";
    out << std::string(offset, ' ') << "Symbols:\n";
    for (const auto& [name, value] : symbols_) {
        out << std::string(offset, ' ') << name << ", offset: " << symbol_offset_.at(name) << "\n";
    }
    for (auto layer: children_) {
        layer->Print(out, offset + 1);
    }
}
