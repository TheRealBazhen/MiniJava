#include <irtree/blocks/block_sequence/block_sequence.h>

#include <irtree/nodes/expression/name.h>
#include <irtree/nodes/statement/conditional_jump.h>
#include <irtree/nodes/statement/jump.h>
#include <irtree/visitors/instruction_selector/instruction_selector.h>
#include <irtree/visitors/printer/printer.h>

namespace IR {
ConditionalOperatorType InvertCondition(ConditionalOperatorType op) {
    if (op == ConditionalOperatorType::GE) {
        return ConditionalOperatorType::LT;
    } else if (op == ConditionalOperatorType::GT) {
        return ConditionalOperatorType::LE;
    } else if (op == ConditionalOperatorType::EQ) {
        return ConditionalOperatorType::NE;
    } else if (op == ConditionalOperatorType::NE) {
        return ConditionalOperatorType::EQ;
    } else if (op == ConditionalOperatorType::LE) {
        return ConditionalOperatorType::GT;
    } else {
        return ConditionalOperatorType::GE;
    }
}

void BlockSequence::AddStatement(std::shared_ptr<Statement> statement) {
    auto label = std::dynamic_pointer_cast<LabelStatement>(statement);
    auto jump = std::dynamic_pointer_cast<JumpStatement>(statement);
    auto cond_jump = std::dynamic_pointer_cast<ConditionalJumpStatement>(statement);

    if (!block_incomplete_ && !label) {
        // Unreachable code - ignore
        return;
    }

    if (label) {
        if (block_incomplete_) {
            // Complete previous block
            blocks_.back()->AddStatement(
                std::make_shared<JumpStatement>(std::make_shared<NameExpression>(label->label))
            );
        } else if (blocks_.size() > 0) {
            /// TODO: think more - seems like useless, block exchanging will perform better
            // Check previous block for conditional jump
            auto last_cond_jump =
                std::dynamic_pointer_cast<ConditionalJumpStatement>(blocks_.back()->GetStatementList().back());
            if (last_cond_jump) {
                if (last_cond_jump->true_label.ToString() == label->label.ToString()) {
                    // Exchange true and false labels
                    std::swap(last_cond_jump->true_label, last_cond_jump->false_label);
                    last_cond_jump->cond_operator = InvertCondition(last_cond_jump->cond_operator);
                } else if (last_cond_jump->false_label.ToString() != label->label.ToString()) {
                    // Insert additional block
                    Label trampoline;
                    last_cond_jump->false_label = trampoline;
                    AddStatement(std::make_shared<LabelStatement>(trampoline));
                    AddStatement(std::make_shared<JumpStatement>(std::make_shared<NameExpression>(trampoline)));
                }
            }
        }
        // Start new block
        blocks_.push_back(std::make_shared<CodeBlock>(label));
        blocks_map_.emplace(label->label.ToString(), blocks_.back());
        block_incomplete_ = true;
    } else if (jump || cond_jump) {
        // Close current block
        blocks_.back()->AddStatement(statement);
        block_incomplete_ = false;
    } else {
        // Add statement to current block
        blocks_.back()->AddStatement(statement);
    }
}

void BlockSequence::AddBlock(std::shared_ptr<CodeBlock> block) {
    blocks_.push_back(block);
    blocks_map_.emplace(block->GetLabel().ToString(), block);
}

std::shared_ptr<CodeBlock> BlockSequence::GetBlock(const Label& label) const {
    return blocks_map_.at(label.ToString());
}

std::vector<std::shared_ptr<CodeBlock>> BlockSequence::GetBlockList() const {
    return blocks_;
}

std::vector<std::shared_ptr<ASM::Instruction>> BlockSequence::TranslateToASM() const {
    auto translator = std::make_shared<InstructionSelector>();

    for (auto block : blocks_) {
        for (auto stmt : block->GetStatementList()) {
            stmt->Accept(translator);
        }
    }
    return translator->GetInstructions();
}

std::ostream& operator<<(std::ostream& out, const BlockSequence& blocks) {
    auto printer = std::make_shared<TreePrinter>(out, true);

    for (auto block : blocks.blocks_) {
        out << "Block '" << block->GetLabel().ToString() << "' start" << std::endl;
        for (auto stmt : block->GetStatementList()) {
            stmt->Accept(printer);
        }
        out << std::endl;
    }

    return out;
}
}
