#ifndef IR_FRAME_H
#define IR_FRAME_H

#include <irtree/function_mechanism/address/address.h>

#include <stack>
#include <string>
#include <unordered_map>

namespace IR {
class Frame {
public:
    Frame(const std::string& class_name, const std::string& method_name);

    void AddVariable(const std::string& symbol);
    std::shared_ptr<Address> GetAddress(const std::string& symbol);

    std::shared_ptr<Address> GetFramePointer();
    std::shared_ptr<Address> GetReturnAddress();
    std::shared_ptr<Address> GetReturnValueAddress();
    std::shared_ptr<Address> GetThisPointer();

    void BeginScope();
    void EndScope();
private:
    size_t GetVariableOffset();
    size_t GetFieldOffset();

    void AddField(const std::string& symbol);

private:
    std::unordered_map<std::string, std::stack<std::shared_ptr<Address>>> addresses_;

    std::stack<std::string> symbols_;

    size_t word_size_ = 4;

    size_t fields_number_ = 0;
    size_t variables_number_ = 0;

    // Reserved symbols
    static const std::string frame_pointer_;
    static const std::string return_address_;
    static const std::string return_value_address_;
    static const std::string this_;
};
}

#endif
