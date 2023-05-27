#include "eval_env.h"
#include "error.h"
#include "builtins.h"
#include "forms.h"
#include <algorithm>
#include <iterator>

std::shared_ptr<EvalEnv> EvalEnv::createGlobal(
    std::shared_ptr<EvalEnv> _parent) {
    std::shared_ptr<EvalEnv> env_ptr{new EvalEnv(_parent)};
    return env_ptr;
}

std::shared_ptr<EvalEnv> EvalEnv::createChild(const std::deque<string>& params,
    const std::deque<ValuePtr>& args) {
    std::shared_ptr<EvalEnv> env_ptr{new EvalEnv(shared_from_this())};
    auto param_ptr = params.begin();
    auto arg_ptr = args.begin();
    while (param_ptr != params.end() && arg_ptr != args.end()) {
        env_ptr->defineBinding(*param_ptr, *arg_ptr);
        param_ptr++;
        arg_ptr++;
    }
    return env_ptr;
}
std::deque<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::deque<ValuePtr> result{};
    std::ranges::transform(expr->toDeque(), std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}
ValuePtr EvalEnv::apply(ValuePtr proc, std::deque<ValuePtr> args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return static_cast<BuiltinProcValue&>(*proc).apply(args, *this);
    } 
    else if (typeid(*proc) == typeid(LambdaValue)) {
        return static_cast<LambdaValue&>(*proc).apply(args);
    }
    else {
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
        if (SPECIAL_FORMS.contains(*name)) 
            return expr;
        else 
            return lookupBinding(*name);
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

                    else {
                        ValuePtr proc = this->eval(expr_pair->getCar());
                        return this->apply(proc,
                                           this->evalList(expr_pair->getCdr()));
                    }
                }
                // Pure List
                else {
                    ValuePtr proc = this->eval(expr_pair->getCar());
                    return this->apply(proc,
                                        this->evalList(expr_pair->getCdr()));
                }
            } 
            else {
                throw LispError("Can't evaluate a pair.");
            } 
        }
    }
}


bool EvalEnv::defineBinding(string name, ValuePtr value) {
    symbolTable[name] = value;
    return true;
}

//Add Builtin Procedure and Form
EvalEnv::EvalEnv(std::shared_ptr<EvalEnv> _parent) : parent{_parent} {
    if (_parent == nullptr) {
        extern const std::unordered_map<std::string, BuiltinFuncType>
            BUILTIN_FUNCS;
        for (auto& func : BUILTIN_FUNCS) {
            symbolTable[func.first] =
                std::make_shared<BuiltinProcValue>(func.second);
        }
    } else {
        symbolTable = _parent->symbolTable;
    }
}

ValuePtr EvalEnv::lookupBinding(const string& name) {
    auto env = shared_from_this();
    while (env != nullptr) {
        if (env->symbolTable.contains(name))
            return env->symbolTable[name];
        else
            env = env->parent;
    }
    throw LispError("Variable " + name + " not defined.");
}
