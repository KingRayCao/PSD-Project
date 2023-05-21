#include "eval_env.h"
#include "error.h"
#include "builtins.h"
#include <algorithm>
#include <iterator>
std::deque<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::deque<ValuePtr> result{};
    std::ranges::transform(expr->toDeque(), std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
} 
ValuePtr EvalEnv::apply(ValuePtr proc, std::deque<ValuePtr> args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return static_cast<BuiltinProcValue&>(*proc).apply(args);
    } else {
        throw LispError("Unimplemented");
    }
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    //Self Evaluating
    if (expr->isSelfEvaluating()) {
        return expr;
    //NilValue
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    //Symbol
    } else if (auto name = expr->asSymbol()) {
        if (symbolTable.contains(*name)) {
            auto value = symbolTable[*name];
            return value;
        } else {
            throw LispError("Variable " + *name + " not defined.");
        }
    } 
    //List
    else {
        try {
            auto vlist = expr->toDeque();
            using namespace std::literals;
            //Define
            if (vlist[0]->asSymbol() == "define"s) {
                if (auto name = vlist[1]->asSymbol()) {
                    symbolTable[*name] = eval(vlist[2]);
                    return std::make_shared<NilValue>();
                } else {
                    throw LispError("Malformed define.");
                }
            } 
            //TODO: Special Form

            // Pure List
            else {
                ValuePtr proc = this->eval(vlist[0]);
                if (auto expr_pair = dynamic_cast<const PairValue*>(expr.get())) {
                    return this->apply(proc, this->evalList(expr_pair->right));
                }

            }

        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}


//Add Builtin Procedure
EvalEnv::EvalEnv() {
    auto builtins = builtinFunc();
    for (auto& func : builtins) {
        symbolTable[func.first] =
            std::make_shared<BuiltinProcValue>(func.second);
    }
    
}
