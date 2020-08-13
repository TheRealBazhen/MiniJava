#include <parser/driver.h>

#include <visitors/irtee_builder/irtee_builder.h>
#include <visitors/tree_printer/tree_printer.h>
#include <visitors/symbol_tree_builder/symbol_tree_builder.h>

#include <irtree/visitors/printer/printer.h>

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