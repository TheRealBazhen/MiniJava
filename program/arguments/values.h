#ifndef AGR_VALUES
#define AGR_VALUES

#include <program/expression/expression.h>

#include <vector>

class ArgumentValues : public Node, public std::enable_shared_from_this<ArgumentValues> {
public:
    void AddValue(std::shared_ptr<Expression> value);

    void Accept(std::shared_ptr<Visitor> visitor) override;

    std::vector<std::shared_ptr<Expression>> values;
};

#endif
