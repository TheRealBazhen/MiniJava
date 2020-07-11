#include <parser/driver.h>

#include <visitors/tree_printer/tree_printer.h>
#include <visitors/interpreter/interpreter.h>

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
            std::cout << "Executing interpreter" << std::endl;
            try {
                driver.program->Accept(std::make_shared<Interpreter>());
            } catch (std::runtime_error& err) {
                std::cout << "Interpreter error: " << err.what() << std::endl;
                result = 1;
            }
            if (result == 0) {
                std::cout << "Interpreting finished" << std::endl;
            }
        } else {
            result = 1;
        }
    }

    return result;
}