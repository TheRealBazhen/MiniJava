#ifndef COMPLEX_H
#define COMPLEX_H

#include <program/statement/statement.h>

#include <memory>

class ComplexStatement : public Statement, public std::enable_shared_from_this<ComplexStatement> {
public:
    ComplexStatement(std::shared_ptr<StatementList> statement_list);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::shared_ptr<StatementList> statement_list;
};

#endif