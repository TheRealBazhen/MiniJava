#include <irtree/function_mechanism/frame/frame.h>

#include <class_storage/class_storage.h>

namespace IR {
const std::string Frame::frame_pointer_ = "%fp";
const std::string Frame::return_address_ = "%return";
const std::string Frame::return_value_address_ = "%return_value";
const std::string Frame::this_ = "this";

Frame::Frame(const std::string& class_name, const std::string& method_name) {
    ClassStorage& storage = ClassStorage::GetInstance();

    addresses_[frame_pointer_].push(std::make_shared<RegisterAddress>(Temporary(frame_pointer_)));
    addresses_[return_value_address_].push(std::make_shared<RegisterAddress>(Temporary(return_value_address_)));
    AddVariable(return_address_);

    if (class_name != "@main_class") {
        AddVariable(this_);
        auto class_descr = storage.GetClassEntry(class_name);
        auto fields = class_descr->GetFieldList();
        for (const auto& [type, name] : fields) {
            AddField(name);
        }
        auto method_descr = class_descr->GetMethod(method_name);
        for (const auto& [type, name] : method_descr.arguments) {
            AddVariable(name);
        }
    }
}

void Frame::AddVariable(const std::string& symbol) {
    addresses_[symbol].push(std::make_shared<OffsetAddress>(GetFramePointer(), GetVariableOffset()));
    symbols_.push(symbol);
    ++variables_number_;
}

std::shared_ptr<Address> Frame::GetAddress(const std::string& symbol) {
    return addresses_[symbol].top();
}

std::shared_ptr<Address> Frame::GetFramePointer() {
    return GetAddress(frame_pointer_);
}

std::shared_ptr<Address> Frame::GetReturnAddress() {
    return GetAddress(return_address_);
}

std::shared_ptr<Address> Frame::GetReturnValueAddress() {
    return GetAddress(return_value_address_);
}

std::shared_ptr<Address> Frame::GetThisPointer() {
    return GetAddress(this_);
}

void Frame::BeginScope() {
    symbols_.push("{");
}

void Frame::EndScope() {
    while (!symbols_.empty()) {
        auto symbol = symbols_.top();
        symbols_.pop();
        if (symbol == "{") {
            break;
        }
        addresses_[symbol].pop();
        --variables_number_;
    }
}

size_t Frame::GetVariableOffset() {
    return variables_number_ * word_size_;
}

size_t Frame::GetFieldOffset() {
    return fields_number_ * word_size_;
}

void Frame::AddField(const std::string& symbol) {
    addresses_[symbol].push(std::make_shared<OffsetAddress>(GetThisPointer(), GetFieldOffset()));
    ++fields_number_;
}
}