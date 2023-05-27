#include "forms.h"
#include "error.h"
const std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS{
    {"define", defineForm},
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},       
    {"lambda", lambdaForm},
    {"cond", condForm},     
    {"begin", beginForm},   
    {"let", letForm},
    {"quasiquote", quasiquoteForm}
};
ValuePtr defineForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    if (auto name = args[0]->asSymbol()) {
        env.defineBinding(*name, env.eval(args[1]));
        return std::make_shared<NilValue>();
    }
    else {
        auto func = static_cast<PairValue&>(*args[0]);
        auto fname = func.getCar()->asSymbol();
        auto params = func.getCdr()->toDeque();
        auto body = args;
        body.pop_front();
        env.defineBinding(*fname, std::make_shared<LambdaValue>(params, body, env.shared_from_this()));
        return std::make_shared<NilValue>();
    }
    throw LispError("Unimplemented");

}
ValuePtr quoteForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    return args[0];
}
ValuePtr ifForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    auto condition = env.eval(args[0]);
    if (typeid(*condition) == typeid(BooleanValue) &&
        static_cast<BooleanValue&>(*condition).getValue() == false) {
        return env.eval(args[2]);
    }
    else {
        return env.eval(args[1]);
    }
}
ValuePtr andForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    for (auto& arg : args) {
        auto condition = env.eval(arg);
        if (typeid(*condition) == typeid(BooleanValue) &&
            static_cast<BooleanValue&>(*condition).getValue() == false) {
            return condition;
        }
    }
    if (args.size() == 0) 
        return std::make_shared<BooleanValue>(true);
    else
        return env.eval(args[args.size() - 1]);
}
ValuePtr orForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    for (auto& arg : args) {
        auto condition = env.eval(arg);
        if (typeid(*condition) == typeid(BooleanValue) &&
            static_cast<BooleanValue&>(*condition).getValue() == false) {
            continue;
        }
        return condition;
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    auto params = static_cast<PairValue&>(*args[0]).toDeque();
    auto body = args;
    body.pop_front();
    return std::make_shared<LambdaValue>(params, body, env.shared_from_this());
}

ValuePtr condForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    ValuePtr ret;
    for (auto clause = args.begin(); clause != args.end(); ++clause) {
        if (auto symbol = static_cast<PairValue&>(**clause).getCar()->asSymbol()) {
            if (*symbol == "else")
                if (clause != args.end() - 1)
                    throw LispError("Else must be in the Last Clause.");
                else {
                    auto exprs =
                        static_cast<PairValue&>(**clause).getCdr()->toDeque();
                    if (exprs.empty())
                        throw LispError("Else doesn't include expressions.");
                    else
                        for (auto& expr : exprs) ret = env.eval(expr);
                    break;
                }
        } else {
            auto condition = env.eval(static_cast<PairValue&>(**clause).getCar());
            if (typeid(*condition) == typeid(BooleanValue) &&
                (!static_cast<BooleanValue&>(*condition).getValue()))
                continue;
            auto exprs = static_cast<PairValue&>(**clause).getCdr()->toDeque();
            if (exprs.empty()) 
                ret = condition;
            else
                for (auto& expr : exprs) 
                    ret = env.eval(expr);
            break;
        }
    }
    return ret;
}
ValuePtr beginForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    ValuePtr ret;
    for (auto& clause : args) {
        ret = env.eval(clause);
    }
    return ret;
}
ValuePtr letForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    auto bindings = args[0]->toDeque();
    std::deque<ValuePtr> params;
    std::deque<ValuePtr> values;
    for (auto& binding : bindings) {
        auto binding_deque = binding->toDeque();
        params.push_back(binding_deque[0]);
        values.push_back(env.eval(binding_deque[1]));
    }
    auto body = args;
    body.pop_front();
    auto _lambda =
        std::make_shared<LambdaValue>(params, body, env.shared_from_this());
    return _lambda->apply(values);
}
ValuePtr quasiquoteForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    auto _args = args[0];
    if (!_args->isList()) 
        return _args;
    else {//_args is a list
        auto _args_pair = static_cast<PairValue&>(*_args);
        if (auto symbol = _args_pair.getCar()->asSymbol()) {
            if (*symbol == "unquote") 
                return env.eval(_args_pair.toDeque()[1]);
        }
        auto _args_deque = _args->toDeque();
        std::deque<ValuePtr> res;
        for (auto& _arg : _args_deque) {
            res.push_back(quasiquoteForm({_arg}, env));
        }
        return std::make_shared<PairValue>(res);
    }
}
