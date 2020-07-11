#include "driver.h"
#include "parser.h"

Driver::Driver() :
    scanner(*this), parser(scanner, *this) {
}


int Driver::Parse(const std::string& file) {
    file_name = file;
    location.initialize(&file_name);
    ScanBegin();
    int res = parser();
    ScanEnd();
    return res;
}

void Driver::ScanBegin() {
    if (file_name.empty () || file_name == "-") {
    } else {
        stream.open(file_name);
        //std::cout << file_name << std::endl;
        scanner.yyrestart(&stream);
    }
}

void Driver::ScanEnd() {
    stream.close();
}

