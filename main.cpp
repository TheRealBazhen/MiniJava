#include <parser/driver.h>

#include <visitors/irtee_builder/irtee_builder.h>
#include <visitors/tree_printer/tree_printer.h>
#include <visitors/symbol_tree_builder/symbol_tree_builder.h>

#include <irtree/visitors/printer/printer.h>
#include <irtree/visitors/call_transformer/call_transformer.h>
#include <irtree/visitors/eseq_mover/eseq_mover.h>
#include <irtree/visitors/linearizer/linearizer.h>
#include <irtree/visitors/block_builder/block_builder.h>
#include <irtree/blocks/trace/trace.h>

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
    int result = 0;
    Driver driver;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        result = 1;
    } else {
        if (!driver.Parse(argv[1])) {
            std::cout << "Program syntax tree:" << std::endl;
            driver.program->Accept(std::make_shared<TreePrinter>(std::cout));

            try {
                std::cout << "Building symbol tree:" << std::endl;
                auto symbol_tree_builder = std::make_shared<SymbolTreeBuilder>();
                driver.program->Accept(symbol_tree_builder);
                auto symbol_tree = symbol_tree_builder->GetSymbolTree();
                std::cout << symbol_tree;

                std::cout << "Building IR tree:" << std::endl;
                auto irtree_builder = std::make_shared<IRTreeBuilder>(symbol_tree);
                driver.program->Accept(irtree_builder);

                std::ofstream ir_out("ir.txt");
                irtree_builder->GetIRTree()->Accept(std::make_shared<IR::TreePrinter>(ir_out));

                std::cout << "Transforming calls" << std::endl;
                auto call_transformer = std::make_shared<IR::CallTransformer>();
                irtree_builder->GetIRTree()->Accept(call_transformer);
                std::ofstream ir_call_out("ir_call_trans.txt");
                call_transformer->GetTree()->Accept(std::make_shared<IR::TreePrinter>(ir_call_out));

                std::cout << "Moving ESEQ to top" << std::endl;
                auto eseq_mover = std::make_shared<IR::ESEQMover>();
                call_transformer->GetTree()->Accept(eseq_mover);
                std::ofstream ir_eseq_out("ir_no_eseq.txt");
                eseq_mover->GetTree()->Accept(
                    std::make_shared<IR::TreePrinter>(ir_eseq_out)
                );

                std::cout << "Linearizing" << std::endl;
                auto linearizer = std::make_shared<IR::Linearizer>();
                eseq_mover->GetTree()->Accept(linearizer);
                std::ofstream ir_linearize_out("ir_linear.txt");
                linearizer->GetTree()->Accept(
                    std::make_shared<IR::TreePrinter>(ir_linearize_out, true)
                );

                std::cout << "Building blocks" << std::endl;
                auto block_builder = std::make_shared<IR::BlockBuilder>();
                linearizer->GetTree()->Accept(block_builder);
                std::ofstream blocks_out("ir_blocks.txt");
                blocks_out << block_builder->GetBlocks();

                std::cout << "Reordering due to tracing" << std::endl;
                IR::TraceBuilder trace_builder;
                trace_builder.ProcessBlocks(block_builder->GetBlocks());
                IR::BlockSequence blocks = trace_builder.MakeBlockSequence();
                std::ofstream reorder_out("ir_trace.txt");
                reorder_out << blocks;
            } catch (std::runtime_error& err) {
                std::cout << "Error: " << err.what() << std::endl;
                result = 1;
            }
        } else {
            result = 1;
        }
    }

    return result;
}