#ifndef DRIVER_H
#define DRIVER_H

#include <map>
#include <string>
#include <fstream>

#include <parser/scanner.h>
#include <parser/parser.h>
#include <program/visitor.h>

class Driver {
    friend class Scanner;
public:
    Driver();
    int Parse(const std::string& file);
    void ScanBegin();
    void ScanEnd();
    
    std::shared_ptr<Program> program;

    std::string file_name;
    yy::location location;
    Scanner scanner;
    yy::parser parser;
private:
    std::ifstream stream;
};

#endif
