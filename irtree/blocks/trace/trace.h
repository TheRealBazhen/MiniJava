#ifndef IR_TRACE_H
#define IR_TRACE_H

#include <irtree/blocks/block_sequence/block_sequence.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace IR {
class Trace {
public:
    void AddBlock(std::shared_ptr<CodeBlock> block);

    std::vector<std::shared_ptr<CodeBlock>> GetBlockList() const;

    Label GetNextBlockLabel() const;

    bool Empty() const;

private:
    std::vector<std::shared_ptr<CodeBlock>> blocks_;
};

class TraceBuilder {
public:
    void ProcessBlocks(const BlockSequence& blocks);

    BlockSequence MakeBlockSequence() const;

private:
    std::unordered_set<std::string> used_blocks_;
    std::vector<Trace> traces_;
};
}

#endif
