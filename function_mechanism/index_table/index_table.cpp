#include <function_mechanism/index_table/index_table.h>

#include <stdexcept>

void IndexTable::CreateVariable(const std::string& symbol) {
    symbols_.push(symbol);
    if (variable_indices_.count(symbol) == 0) {
        variable_indices_[symbol] = {};
    }
}

void IndexTable::PutIndex(const std::string& symbol, size_t index) {
    if (variable_indices_.count(symbol) == 0) {
        throw std::runtime_error("Variable " + symbol + " not declared");
    }
    variable_indices_[symbol].push(index);
}

size_t IndexTable::GetIndex(const std::string& symbol) {
    if (variable_indices_.count(symbol) == 0) {
        throw std::runtime_error("Variable " + symbol + " not declared");
    }
    return variable_indices_[symbol].top();
}

void IndexTable::BeginScope() {
    symbols_.push("{");
}

void IndexTable::EndScope() {
    while (!symbols_.empty()) {
        auto symb = symbols_.top();
        symbols_.pop();
        if (symb == "{") {
            break;
        }
        variable_indices_[symb].pop();
    }
}
