#include <irtree/blocks/trace/trace.h>

#include <irtree/nodes/expression/name.h>
#include <irtree/nodes/statement/conditional_jump.h>
#include <irtree/nodes/statement/jump.h>

#include <stdexcept>

namespace IR {
void Trace::AddBlock(std::shared_ptr<CodeBlock> block) {
    blocks_.push_back(block);
}

std::vector<std::shared_ptr<CodeBlock>> Trace::GetBlockList() const {
    return blocks_;
}

Label Trace::GetNextBlockLabel() const {
    auto stmt = blocks_.back()->GetStatementList().back();
    auto jump = std::dynamic_pointer_cast<JumpStatement>(stmt);
    auto cond_jump = std::dynamic_pointer_cast<ConditionalJumpStatement>(stmt);

    if (jump) {
        auto label = std::dynamic_pointer_cast<NameExpression>(jump->address);
        return label->label;
    } else if (cond_jump) {
        return cond_jump->false_label;
    } else {
        throw std::runtime_error("Incorrect block");
    }
}

bool Trace::Empty() const {
    return blocks_.empty();
}

void TraceBuilder::ProcessBlocks(const BlockSequence& blocks) {
    auto block_list = blocks.GetBlockList();

    for (auto block : block_list) {
        Trace trace;
        Label label = block->GetLabel();
        while (label.ToString() != "@DONE" && used_blocks_.count(label.ToString()) == 0) {
            trace.AddBlock(blocks.GetBlock(label));
            used_blocks_.emplace(label.ToString());
            label = trace.GetNextBlockLabel();
        }
        if (!trace.Empty()) {
            traces_.push_back(trace);
        }
    }
}

BlockSequence TraceBuilder::MakeBlockSequence() const {
    BlockSequence seq;

    for (const auto& trace : traces_) {
        for (auto block : trace.GetBlockList()) {
            seq.AddBlock(block);
        }
    }
    return seq;
}
}
