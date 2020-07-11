#ifndef EXPRESSION
#define EXPRESSION

#include <program/node.h>

class Expression : public Node {
};

class CallableExpr : public Expression {
};

class BooleanExpr : public Expression {
};

#endif