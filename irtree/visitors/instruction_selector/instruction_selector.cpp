#include <irtree/visitors/instruction_selector/instruction_selector.h>

#include <instructions/binary_operations/binary_operations.h>
#include <instructions/jumps/call.h>
#include <instructions/jumps/conditional_jumps.h>
#include <instructions/jumps/jump.h>
#include <instructions/label/label.h>
#include <instructions/movements/movements.h>

#include <stdexcept>

namespace IR {
InstructionSelector::InstructionSelector() : eax_("%eax"), edx_("%edx") {
}

std::vector<std::shared_ptr<ASM::Instruction>> InstructionSelector::GetInstructions() const {
    return instructions_;
}

std::shared_ptr<ASM::Source> InstructionSelector::GetSource(std::shared_ptr<Expression> expr) {
    auto constant = std::dynamic_pointer_cast<ConstantExpression>(expr);
    auto memory_access = std::dynamic_pointer_cast<MemoryAccessExpression>(expr);

    if (constant) {
        return std::make_shared<ASM::Constant>(constant->value);
    } else if (memory_access) {
        auto plus = std::dynamic_pointer_cast<BinaryOperationExpression>(memory_access->address);
        if (plus && plus->type == BinaryOperaionType::PLUS) {
            auto constant_offset = std::dynamic_pointer_cast<ConstantExpression>(plus->rhs);
            if (constant_offset) {
                return std::make_shared<ASM::MemoryAccess>(Visit(plus->lhs), constant_offset->value);
            } else {
                return std::make_shared<ASM::MemoryAccess>(Visit(memory_access->address), 0);
            }
        } else {
            return std::make_shared<ASM::MemoryAccess>(Visit(memory_access->address), 0);
        }
    } else {
        return std::make_shared<ASM::Register>(Visit(expr));
    }
}

Temporary InstructionSelector::Visit(std::shared_ptr<Node> node) {
    node->Accept(shared_from_this());
    return result_register_;
}

void InstructionSelector::Visit(std::shared_ptr<BinaryOperationExpression> expr) {
    auto destination = Visit(expr->lhs);
    auto source = GetSource(expr->rhs);

    if (expr->type == BinaryOperaionType::PLUS) {
        instructions_.push_back(std::make_shared<ASM::Add>(destination, source));
        result_register_ = destination;
    } else if (expr->type == BinaryOperaionType::MINUS) {
        instructions_.push_back(std::make_shared<ASM::Sub>(destination, source));
        result_register_ = destination;
    } else if (expr->type == BinaryOperaionType::MUL) {
        instructions_.push_back(
            std::make_shared<ASM::MoveToRegister>(eax_, std::make_shared<ASM::Register>(destination))
        );
        instructions_.push_back(std::make_shared<ASM::Mul>(source));
        result_register_ = edx_;
    } else if (expr->type == BinaryOperaionType::DIV) {
        instructions_.push_back(
            std::make_shared<ASM::MoveToRegister>(eax_, std::make_shared<ASM::Register>(destination))
        );
        instructions_.push_back(std::make_shared<ASM::Div>(source));
        result_register_ = eax_;
    } else if (expr->type == BinaryOperaionType::REM) {
        instructions_.push_back(
            std::make_shared<ASM::MoveToRegister>(eax_, std::make_shared<ASM::Register>(destination))
        );
        instructions_.push_back(std::make_shared<ASM::Div>(source));
        result_register_ = edx_;
    } else {
        throw std::runtime_error("Operation not supported");
    }
}

void InstructionSelector::Visit(std::shared_ptr<CallExpression> expr) {
    auto function_name = std::dynamic_pointer_cast<NameExpression>(expr->function);
    if (!function_name) {
        throw std::runtime_error("Call only from label");
    }
    std::vector<std::shared_ptr<ASM::Source>> arguments;
    for (auto arg : expr->arguments->expressions) {
        arguments.push_back(GetSource(arg));
    }
    /// TODO: move arguments to stack?
    instructions_.push_back(std::make_shared<ASM::Call>(function_name->label));
    result_register_ = eax_;
}

void InstructionSelector::Visit(std::shared_ptr<ConstantExpression> expr) {
    Temporary temp;
    instructions_.push_back(std::make_shared<ASM::MoveToRegister>(temp, std::make_shared<ASM::Constant>(expr->value)));
    result_register_ = temp;
}

void InstructionSelector::Visit(std::shared_ptr<ESEQExpression> expr) {
    throw std::runtime_error("Canonize IR before translating to assembler");
}

void InstructionSelector::Visit(std::shared_ptr<MemoryAccessExpression> expr) {
    Temporary temp;
    instructions_.push_back(std::make_shared<ASM::MoveToRegister>(temp, GetSource(expr)));
    result_register_ = temp;
}

void InstructionSelector::Visit(std::shared_ptr<NameExpression> expr) {
    // Ignore
}

void InstructionSelector::Visit(std::shared_ptr<TemporaryExpression> expr) {
    result_register_ = expr->temp;
}

void InstructionSelector::Visit(std::shared_ptr<ConditionalJumpStatement> stmt) {
    auto lhs = GetSource(stmt->lhs);
    auto rhs = GetSource(stmt->rhs);
    instructions_.push_back(std::make_shared<ASM::Compare>(lhs, rhs));
    instructions_.push_back(std::make_shared<ASM::ConditionalJump>(stmt->cond_operator, stmt->true_label));
    /// TODO: May be not necessary?
    instructions_.push_back(std::make_shared<ASM::Jump>(stmt->false_label));
}

void InstructionSelector::Visit(std::shared_ptr<ExpressionStatement> stmt) {
    Visit(stmt->expression);
}

void InstructionSelector::Visit(std::shared_ptr<JumpStatement> stmt) {
    auto name = std::dynamic_pointer_cast<NameExpression>(stmt->address);
    if (!name) {
        throw std::runtime_error("Jump only to label");
    }
    instructions_.push_back(std::make_shared<ASM::Jump>(name->label));
}

void InstructionSelector::Visit(std::shared_ptr<LabelStatement> stmt) {
    instructions_.push_back(std::make_shared<ASM::Label>(stmt->label));
}

void InstructionSelector::Visit(std::shared_ptr<MoveStatement> stmt) {
    auto dst = GetSource(stmt->destination);
    auto src = GetSource(stmt->source);
    
    auto dst_reg = std::dynamic_pointer_cast<ASM::Register>(dst);
    auto dst_mem = std::dynamic_pointer_cast<ASM::MemoryAccess>(dst);
    if (dst_reg) {
        instructions_.push_back(std::make_shared<ASM::MoveToRegister>(dst_reg->temp, src));
    } else if (dst_mem) {
        instructions_.push_back(std::make_shared<ASM::MoveToMemory>(dst_mem, src));
    } else {
        throw std::runtime_error("Can move only to register or memory");
    }
}

void InstructionSelector::Visit(std::shared_ptr<SequenceStatement> stmt) {
    Visit(stmt->first);
    Visit(stmt->second);
}

void InstructionSelector::Visit(std::shared_ptr<ExpressionList> list) {
    // Ignore
}
}
