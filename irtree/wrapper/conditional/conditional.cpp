#include <irtree/wrapper/conditional/conditional.h>

#include <irtree/nodes/expression/constant.h>
#include <irtree/nodes/expression/eseq.h>
#include <irtree/nodes/expression/temporary.h>
#include <irtree/nodes/statement/expression.h>
#include <irtree/nodes/statement/label.h>
#include <irtree/nodes/statement/move.h>
#include <irtree/nodes/statement/sequence.h>

namespace IR {
std::shared_ptr<Expression> ConditionalWrapper::ToExpression() {
    auto temp = std::make_shared<TemporaryExpression>(Temporary());
    Label true_label;
    Label false_label;

    return std::make_shared<ESEQExpression>(
        std::make_shared<SequenceStatement>(
            std::make_shared<MoveStatement>(temp, std::make_shared<ConstantExpression>(1)),
            std::make_shared<SequenceStatement>(
                ToConditional(true_label, false_label),
                std::make_shared<SequenceStatement>(
                    std::make_shared<LabelStatement>(false_label),
                    std::make_shared<SequenceStatement>(
                        std::make_shared<MoveStatement>(temp, std::make_shared<ConstantExpression>(0)),
                        std::make_shared<LabelStatement>(true_label)
                    )
                )
            )
        ),
        temp
    );
}

std::shared_ptr<Statement> ConditionalWrapper::ToStatement() {
    return std::make_shared<ExpressionStatement>(ToExpression());
}
}
