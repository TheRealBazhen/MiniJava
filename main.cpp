#include <iostream>
#include <parser/driver.h>
#include <visitors/tree_printer/tree_printer.h>

int main(int argc, char** argv) {
    int result = 0;
    Driver driver;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    } else {
        if (!driver.Parse(argv[1])) {
            std::cout << "Program syntax tree:" << std::endl;
            driver.program->Accept(std::make_shared<TreePrinter>(std::cout));
        } else {
            result = 1;
        }
    }

    return result;
}