#ifndef DRIVER_H
#define DRIVER_H

#include <map>
#include <string>
#include <fstream>
#include "scanner.h"
#include "parser.h"

class Driver {
    friend class Scanner;
public:
    Driver();
    int Parse(const std::string& file);
    void ScanBegin();
    void ScanEnd();
    
    std::map<std::string, int> variables;

    std::string file_name;
    yy::location location;
    Scanner scanner;
    yy::parser parser;
private:
    std::ifstream stream;
};

#endif
