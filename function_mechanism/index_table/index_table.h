#ifndef INDEX_TABLE_H
#define INDEX_TABLE_H

#include <stack>
#include <string>
#include <unordered_map>

class IndexTable {
public:
    void CreateVariable(const std::string& symbol);

    void PutIndex(const std::string& symbol, size_t index);
    size_t GetIndex(const std::string& symbol);

    void BeginScope();
    void EndScope();

private:
    std::unordered_map<std::string, std::stack<size_t>> variable_indices_;
    std::stack<std::string> symbols_;
};

#endif
