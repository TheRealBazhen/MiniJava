#include <function_mechanism/frame/frame.h>

Frame::Frame(
    const std::vector<std::shared_ptr<Type>>& fields,
    const std::vector<std::shared_ptr<Type>>& parameters,
    Frame* parent
) : fields_(fields), parameters_(parameters), parent_(parent) {
}

size_t Frame::AllocVariable() {
    variables_.push_back(nullptr);
    return fields_.size() + parameters_.size() + variables_.size() - 1;
}

std::shared_ptr<Type> Frame::GetValue(size_t idx) {
    if (idx < fields_.size()) {
        return fields_.at(idx);
    } else if (idx < fields_.size() + parameters_.size()) {
        return parameters_.at(idx - fields_.size());
    } else {
        return variables_.at(idx - fields_.size() - parameters_.size());
    }
}

void Frame::SetValue(size_t idx, std::shared_ptr<Type> value) {
    if (idx < fields_.size()) {
        fields_.at(idx) = value;
    } else if (idx < fields_.size() + parameters_.size()) {
        parameters_.at(idx - fields_.size()) = value;
    } else {
        variables_.at(idx - fields_.size() - parameters_.size()) = value;
    }
}

void Frame::AllocScope() {
    scope_offsets_.push(variables_.size());
}

void Frame::DeallocScope() {
    variables_.resize(scope_offsets_.top());
    scope_offsets_.pop();
}

void Frame::SetParentReturnValue(std::shared_ptr<Type> value) {
    if (parent_) {
        parent_->return_value_ = value;
    }
}

std::shared_ptr<Type> Frame::GetReturnValue() {
    return return_value_;
}
