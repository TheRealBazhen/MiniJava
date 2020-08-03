#include <parser/driver.h>

#include <visitors/tree_printer/tree_printer.h>
#include <visitors/function_caller/function_caller.h>
#include <visitors/symbol_tree_builder/symbol_tree_builder.h>

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
                auto tree_builder = std::make_shared<SymbolTreeBuilder>();
                driver.program->Accept(tree_builder);
                auto symbol_tree = tree_builder->GetSymbolTree();
                std::cout << symbol_tree;

                ClassStorage& storage = ClassStorage::GetInstance();
                std::cout << "Executing main\n";
                try {
                    auto main_caller =
                        std::make_shared<FunctionCaller>(
                            symbol_tree,
                            std::dynamic_pointer_cast<ClassType>(storage.MakeValue("@main_class")),
                            "main"
                        );
                    driver.program->main_class->Accept(main_caller);
                } catch (std::runtime_error& err) {
                    std:: cout << "Error: " << err.what() << std::endl;
                    result = 1;
                }
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