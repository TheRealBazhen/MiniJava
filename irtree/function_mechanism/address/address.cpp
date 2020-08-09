#include <irtree/function_mechanism/address/address.h>

#include <irtree/nodes/expression/binary_operation.h>
#include <irtree/nodes/expression/constant.h>
#include <irtree/nodes/expression/memory_access.h>
#include <irtree/nodes/expression/temporary.h>

namespace IR {
RegisterAddress::RegisterAddress(const Temporary& temp) : temp_(temp) {
}

std::shared_ptr<Expression> RegisterAddress::ToExpression() {
    return std::make_shared<TemporaryExpression>(temp_);
}

OffsetAddress::OffsetAddress(std::shared_ptr<Address> base_address, size_t offset)
    : base_address_(base_address), offset_(offset) {
}

std::shared_ptr<Expression> OffsetAddress::ToExpression() {
    auto offset_expr = base_address_->ToExpression();

    if (offset_ != 0) {
        offset_expr = std::make_shared<BinaryOperationExpression>(
            BinaryOperaionType::PLUS,
            offset_expr,
            std::make_shared<ConstantExpression>(offset_)
        );
    }
    return std::make_shared<MemoryAccessExpression>(offset_expr);
}
}
