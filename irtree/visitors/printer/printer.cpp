#include <irtree/visitors/printer/printer.h>

namespace IR {
std::string ToString(BinaryOperaionType op) {
    if (op == BinaryOperaionType::PLUS) {
        return "PLUS";
    } else if (op == BinaryOperaionType::MINUS) {
        return "MINUS";
    } else if (op == BinaryOperaionType::MUL) {
        return "MUL";
    } else if (op == BinaryOperaionType::DIV) {
        return "DIV";
    } else if (op == BinaryOperaionType::REM) {
        return "REM";
    } else if (op == BinaryOperaionType::AND) {
        return "AND";
    } else {
        return "OR";
    }
}

std::string ToString(ConditionalOperatorType op) {
    if (op == ConditionalOperatorType::GT) {
        return "GT";
    } else if (op == ConditionalOperatorType::GE) {
        return "GE";
    } else if (op == ConditionalOperatorType::LT) {
        return "LT";
    } else if (op == ConditionalOperatorType::LE) {
        return "LE";
    } else if (op == ConditionalOperatorType::EQ) {
        return "EQ";
    } else {
        return "NE";
    }
}

TreePrinter::TreePrinter(std::ostream& output, bool highlight_sequences)
    : output_(output), highlight_sequences_(highlight_sequences) {
}

void TreePrinter::PutLine(const std::string& line) {
    output_ << std::string(2 * offset_, ' ') << line << std::endl;
}

void TreePrinter::Visit(std::shared_ptr<BinaryOperationExpression> expr) {
    PutLine("Binary operation, type: " + ToString(expr->type));
    ++offset_;
    PutLine("lhs:");
    ++offset_;
    expr->lhs->Accept(shared_from_this());
    --offset_;
    PutLine("rhs:");
    ++offset_;
    expr->rhs->Accept(shared_from_this());
    --offset_;
    --offset_;
}

void TreePrinter::Visit(std::shared_ptr<CallExpression> expr) {
    PutLine("Call");
    ++offset_;
    PutLine("Function:");
    ++offset_;
    expr->function->Accept(shared_from_this());
    --offset_;
    PutLine("Arguments:");
    ++offset_;
    expr->arguments->Accept(shared_from_this());
    --offset_;
    --offset_;
}

void TreePrinter::Visit(std::shared_ptr<ConstantExpression> expr) {
    PutLine("Constant: " + std::to_string(expr->value));
}

void TreePrinter::Visit(std::shared_ptr<ESEQExpression> expr) {
    PutLine("ESEQ");
    ++offset_;
    PutLine("Side effect:");
    ++offset_;
    expr->side_effect->Accept(shared_from_this());
    --offset_;
    PutLine("Value:");
    ++offset_;
    expr->value->Accept(shared_from_this());
    --offset_;
    --offset_;
}

void TreePrinter::Visit(std::shared_ptr<MemoryAccessExpression> expr) {
    PutLine("Memory access from address:");
    ++offset_;
    expr->address->Accept(shared_from_this());
    --offset_;
}

void TreePrinter::Visit(std::shared_ptr<NameExpression> expr) {
    PutLine("Name expression: " + expr->label.ToString());
}

void TreePrinter::Visit(std::shared_ptr<TemporaryExpression> expr) {
    PutLine("Virtual register: " + expr->temp.ToString());
}

void TreePrinter::Visit(std::shared_ptr<ConditionalJumpStatement> stmt) {
    PutLine("Conditional jump");
    ++offset_;
    PutLine("Comparison: " + ToString(stmt->cond_operator));
    PutLine("lhs:");
    ++offset_;
    stmt->lhs->Accept(shared_from_this());
    --offset_;
    PutLine("rhs:");
    ++offset_;
    stmt->rhs->Accept(shared_from_this());
    --offset_;
    PutLine("true label: " + stmt->true_label.ToString());
    PutLine("false label: " + stmt->false_label.ToString());
    --offset_;
}

void TreePrinter::Visit(std::shared_ptr<ExpressionStatement> stmt) {
    PutLine("Expression statement");
    ++offset_;
    stmt->expression->Accept(shared_from_this());
    --offset_;
}

void TreePrinter::Visit(std::shared_ptr<JumpStatement> stmt) {
    PutLine("Jump to address:");
    ++offset_;
    stmt->address->Accept(shared_from_this());
    --offset_;
}

void TreePrinter::Visit(std::shared_ptr<LabelStatement> stmt) {
    PutLine("Make label: " + stmt->label.ToString());
}

void TreePrinter::Visit(std::shared_ptr<MoveStatement> stmt) {
    PutLine("Move statement");
    ++offset_;
    PutLine("Destination");
    ++offset_;
    stmt->destination->Accept(shared_from_this());
    --offset_;
    PutLine("Source");
    ++offset_;
    stmt->source->Accept(shared_from_this());
    --offset_;
    --offset_;
}

void TreePrinter::Visit(std::shared_ptr<SequenceStatement> stmt) {
    if (highlight_sequences_) {
        PutLine("Sequence of statements");
        ++offset_;
        PutLine("First:");
        ++offset_;
        stmt->first->Accept(shared_from_this());
        --offset_;
        PutLine("Second:");
        ++offset_;
        stmt->second->Accept(shared_from_this());
        --offset_;
        --offset_;
    } else {
        stmt->first->Accept(shared_from_this());
        stmt->second->Accept(shared_from_this());
    }
}

void TreePrinter::Visit(std::shared_ptr<ExpressionList> list) {
    for (auto expr : list->expressions) {
        expr->Accept(shared_from_this());
    }
    if (list->expressions.empty()) {
        PutLine("None");
    }
}
}
