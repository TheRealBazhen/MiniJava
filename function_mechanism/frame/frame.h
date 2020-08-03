#ifndef FRAME_H
#define FRAME_H

#include <types/type.h>

#include <memory>
#include <stack>
#include <vector>

class Frame {
public:
    Frame(
        const std::vector<std::shared_ptr<Type>>& fields,
        const std::vector<std::shared_ptr<Type>>& parameters,
        Frame* parent = nullptr
    );

    size_t AllocVariable();

    std::shared_ptr<Type> GetValue(size_t idx);
    void SetValue(size_t idx, std::shared_ptr<Type> value);

    void AllocScope();
    void DeallocScope();

    void SetParentReturnValue(std::shared_ptr<Type> value);
    std::shared_ptr<Type> GetReturnValue();

private:
    std::vector<std::shared_ptr<Type>> fields_;
    std::vector<std::shared_ptr<Type>> parameters_;
    std::vector<std::shared_ptr<Type>> variables_;

    std::stack<size_t> scope_offsets_;

    std::shared_ptr<Type> return_value_;
    Frame* parent_;

};

#endif
