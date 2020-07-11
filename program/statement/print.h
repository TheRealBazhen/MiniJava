#ifndef PRINT_H
#define PRINT_H

#include <program/expression/expression.h>
#include <program/statement/statement.h>

class PrintStatement : public Statement, public std::enable_shared_from_this<PrintStatement> {
public:
    PrintStatement(std::shared_ptr<Expression> value);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<Expression> value;
};

#endif