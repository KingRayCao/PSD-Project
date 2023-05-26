#include "eval_env.h"
#include "error.h"
#include "builtins.h"
#include "forms.h"
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
    extern const std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS;
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
        if (auto expr_pair = dynamic_cast<const PairValue*>(expr.get())) {
            if (expr_pair->isList()) {
                if (auto name = expr_pair->getCar()->asSymbol()) {
                    // Special Form
                    if (SPECIAL_FORMS.contains(*name)) {
                        auto form = SPECIAL_FORMS.find(*name)->second;
                        return form(expr_pair->getCdr()->toDeque(), *this);
                    }
                    // Pure List
                    else {
                        ValuePtr proc = this->eval(expr_pair->getCar());
                        return this->apply(proc,
                                           this->evalList(expr_pair->getCdr()));
                    }
                } 
                else {
                    throw LispError("Can't evaluate the list");
                }
            }
            else {
                throw LispError("Can't evaluate a pair.");
            }
        }
    }
}

bool EvalEnv::add(std::string name, ValuePtr value) {
    symbolTable[name] = value;
    return true;
}

//Add Builtin Procedure and Form
EvalEnv::EvalEnv() {
    extern const std::unordered_map<std::string, BuiltinFuncType> BUILTIN_FUNCS;
    for (auto& func : BUILTIN_FUNCS) {
        symbolTable[func.first] =
            std::make_shared<BuiltinProcValue>(func.second);
    }
}

