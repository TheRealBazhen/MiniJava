#ifndef IR_BLOCK_SEQUENCE_H
#define IR_BLOCK_SEQUENCE_H

#include <irtree/blocks/block/block.h>

#include <iostream>
#include <string>
#include <unordered_map>

namespace IR {
class BlockSequence {
public:
    void AddStatement(std::shared_ptr<Statement> statement);
    void AddBlock(std::shared_ptr<CodeBlock> block);

    std::shared_ptr<CodeBlock> GetBlock(const Label& label) const;
    std::vector<std::shared_ptr<CodeBlock>> GetBlockList() const;

    friend std::ostream& operator<<(std::ostream& out, const BlockSequence& blocks);

private:
    std::vector<std::shared_ptr<CodeBlock>> blocks_;
    std::unordered_map<std::string, std::shared_ptr<CodeBlock>> blocks_map_;

    bool block_incomplete_ = false;
};
}

#endif
