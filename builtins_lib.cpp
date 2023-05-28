#include "builtins.h"
#include "error.h"

bool checkParam(const std::deque<ValuePtr>& params, int min_n, int max_n=-1) {
    if (params.size() >= min_n)
        if (max_n == -1 || params.size() <= max_n)
            return true;
        else
            throw LispError("Incorrect number of parameters.");
    else
        throw LispError("Incorrect number of parameters.");
}

//核心库
ValuePtr applyFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    return env.apply(params[0], params[1]->toDeque());
}
ValuePtr displayFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    auto v = params[0];
    if (typeid(*v) == typeid(StringValue)) {
        auto s = v->toString();
        std::cout << s.substr(1, s.size() - 2);
    } else
        std::cout << v->toString();
    return std::make_shared<NilValue>();
}
ValuePtr displaylnFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    auto v = params[0];
    if (typeid(*v) == typeid(StringValue)) {
        auto s = v->toString();
        std::cout << s.substr(1, s.size() - 2);
    } else
        std::cout << v->toString();
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}
ValuePtr errorFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 0, 1);
    if (params.empty())
        throw LispError("Error.");
    else
        throw LispError(params[0]->toString());
}
ValuePtr evalFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    return env.eval(params[0]);
}
ValuePtr exitFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 0, 1);
    if (params.empty()) 
        exit(0);
    else
        exit(params[0]->asNumber());
}
ValuePtr newlineFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    if (params.size() != 0)
        throw LispError("Incorrect number of parameters.");
    else
        std::cout << std::endl;
    return std::make_shared<NilValue>();
}
ValuePtr printFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    for (auto v : params) {
        std::cout << v->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

//类型检查库

// 对子与列表操作库
ValuePtr appendFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    if (params.size() == 0)
        return std::make_shared<NilValue>();
    else {
        std::deque<ValuePtr> lists{};
        for (auto& param : params)
            if (param->isList() || param->isNil()) {
                auto list = param->toDeque();
                lists.insert(lists.end(), list.begin(), list.end());
            } else {
                throw LispError("Cannot apply append to a non-list value.");
            }
        return std::make_shared<PairValue>(lists);
    }
}

ValuePtr carFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    if (typeid(*params[0]) != typeid(PairValue))
        throw LispError("Cannot apply car to a non-pair value.");
    else
        return static_cast<PairValue&>(*params[0]).getCar();
}
ValuePtr cdrFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    if (typeid(*params[0]) != typeid(PairValue))
        throw LispError("Cannot apply cdr to a non-pair value.");
    else
        return static_cast<PairValue&>(*params[0]).getCdr();
}
ValuePtr consFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    return std::make_shared<PairValue>(params[0], params[1]);
}

ValuePtr lengthFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    if (params[0]->isNil())
        return std::make_shared<NumericValue>(0);
    else if (!params[0]->isList()) {
        throw LispError("Cannot apply length to a non-list value.");
    } else {
        return std::make_shared<NumericValue>(params[0]->toDeque().size());
    }
}
ValuePtr listFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 0);
    if (params.size() == 0) 
        return std::make_shared<NilValue>();
    return std::make_shared<PairValue>(params);
}
ValuePtr mapFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2);
    auto proc = params[0];
    std::deque<std::deque<ValuePtr>> args;
    int len = -1;
    for (auto arg = params.begin() + 1; arg != params.end(); ++arg) {
        auto a = (*arg)->toDeque();
        args.push_back(a);
        if (len != -1 && a.size() != len)
            throw LispError("Can't apply map to lists of different length.");
        len = a.size();
    }
    std::deque<ValuePtr> res{};
    while (!args[0].empty()) {
        std::deque<ValuePtr> param{};
        for (auto& arg : args) {
            param.push_back(arg[0]);
            arg.pop_front();
        }
        res.push_back(env.apply(proc, param));
    } 
    return std::make_shared<PairValue>(res);
}
ValuePtr filterFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    auto proc = params[0];
    auto param = params[1]->toDeque();
    std::deque<ValuePtr> res{};
    while (!param.empty()) {
        auto ret = env.apply(proc, std::deque<ValuePtr>{param[0]});
        if (!(typeid(*ret) == typeid(BooleanValue) && 
            static_cast<BooleanValue&>(*ret).getValue() == false))
            res.push_back(param[0]);
        param.pop_front();
    }
    return std::make_shared<PairValue>(res);
}
ValuePtr reduceFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    auto proc = params[0];
    auto param = params[1]->toDeque();
    auto param_cdr = static_cast<PairValue&>(*params[1]).getCdr();
    if (param.size() == 0)
        throw LispError("Can't apply reduce to an empty list.");
    if (param.size() == 1)
        return param[0];
    else {
        return env.apply(
            proc, std::deque<ValuePtr>{
                      param[0],
                      reduceFunc(std::deque<ValuePtr>{proc, param_cdr}, env)});
    }
    
}

//算术运算库
ValuePtr addFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    double result = 0;
    for (const auto& i : params) {
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr minusFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 2);
    if (params.size() == 1) {
        auto& i = params[0];
        return std::make_shared<NumericValue>(-i->asNumber());
    } else if (params.size() == 2) {
        auto& x = params[0];
        auto& y = params[1];
        return std::make_shared<NumericValue>(x->asNumber() - y->asNumber());
    }
}
ValuePtr multiplyFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    double result = 1;
    for (const auto& i : params) {
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr divideFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 2);
    if (params.size() == 1) {
        auto& i = params[0];
        if (i->asNumber() == 0) {
            throw LispError("Can't divide by zero.");
        }
        return std::make_shared<NumericValue>(1.0 / i->asNumber());
    } else if (params.size() == 2) {
        auto& x = params[0];
        auto& y = params[1];
        if (y->asNumber() == 0) {
            throw LispError("Can't divide by zero.");
        }
        return std::make_shared<NumericValue>(x->asNumber() / y->asNumber());
    }
}

ValuePtr absFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    return std::make_shared<NumericValue>(abs(params[0]->asNumber()));
}

ValuePtr exptFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    if (params[0]->asNumber() == 0 && params[1]->asNumber() == 0)
        throw LispError("0^0 Undefined.");
    return std::make_shared<NumericValue>(pow(params[0]->asNumber(), params[1]->asNumber()));
}
ValuePtr quotientFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    auto ret = divideFunc(params, env);
    if (ret->asNumber() >= 0) 
        return std::make_shared<NumericValue>(floor(ret->asNumber()));
    else
        return std::make_shared<NumericValue>(ceil(ret->asNumber()));
}
ValuePtr moduloFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    if (y == 0)
        throw LispError("Apply modulo to 0 Undefined.");
    else {
        return std::make_shared<NumericValue>(x - floor(x / y) * y);
    }
}
ValuePtr remainderFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    double x = params[0]->asNumber();
    double y = params[1]->asNumber();
    if (y == 0)
        throw LispError("Apply remainder to 0 Undefined.");
    else {
        return std::make_shared<NumericValue>(x -
                                              y * quotientFunc(params, env)->asNumber());
    }
}

//比较库
ValuePtr eqFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    auto x = params[0];
    auto y = params[1];
    if (typeid(*x) != typeid(*y))
        return std::make_shared<BooleanValue>(false);
    else {
        if (typeid(*x) == typeid(NumericValue) ||
            typeid(*x) == typeid(NilValue) || 
            typeid(*x) == typeid(SymbolValue) ||
            typeid(*x) == typeid(BooleanValue))
            return std::make_shared<BooleanValue>(*x == *y);
        else {
            return std::make_shared<BooleanValue>(x == y);
        }
    }
}
ValuePtr equalFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    auto x = params[0];
    auto y = params[1];
    if (typeid(*x) != typeid(*y))
        return std::make_shared<BooleanValue>(false);
    else {
        if (static_cast<BooleanValue&>(*eqFunc(params, env)).getValue())
            return std::make_shared<BooleanValue>(true);
        else if (typeid(*x) == typeid(StringValue)) 
            return std::make_shared<BooleanValue>(*x == *y);
        else if (typeid(*x) == typeid(PairValue)) {
            auto x_pair = static_cast<const PairValue&>(*x);
            auto y_pair = static_cast<const PairValue&>(*y);
            auto left_eq = equalFunc({x_pair.getCar(), y_pair.getCar()}, env);
            auto right_eq = equalFunc({x_pair.getCdr(), y_pair.getCdr()}, env);
            return std::make_shared<BooleanValue>
                (static_cast<BooleanValue&>(*left_eq).getValue() &&
                    static_cast<BooleanValue&>(*right_eq).getValue());
        } else {
            return std::make_shared<BooleanValue>(false);
        }
            
    }
}
ValuePtr notFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    if (typeid(*params[0]) == typeid(BooleanValue) &&
        (!static_cast<BooleanValue&>(*params[0]).getValue()))
        return std::make_shared<BooleanValue>(true);
    else {
        return std::make_shared<BooleanValue>(false);
    }
}
ValuePtr optEqFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    return std::make_shared<BooleanValue>(params[0]->asNumber() ==
                                          params[1]->asNumber());
}
ValuePtr ltFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    return std::make_shared<BooleanValue>(params[0]->asNumber() <
                                          params[1]->asNumber());
}
ValuePtr gtFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    return std::make_shared<BooleanValue>(params[0]->asNumber() >
                                          params[1]->asNumber());
}
ValuePtr lteFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    return std::make_shared<BooleanValue>(params[0]->asNumber() <=
                                          params[1]->asNumber());
}
ValuePtr gteFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 2, 2);
    return std::make_shared<BooleanValue>(params[0]->asNumber() >=
                                          params[1]->asNumber());
}
ValuePtr evenFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    double x = params[0]->asNumber();
    return std::make_shared<BooleanValue>(floor(x) == ceil(x) &&
                                          abs(int(x)) % 2 == 0);
}
ValuePtr oddFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    double x = params[0]->asNumber();
    return std::make_shared<BooleanValue>(floor(x) == ceil(x) &&
                                          abs(int(x)) % 2 == 1);
}
ValuePtr zeroFunc(const std::deque<ValuePtr>& params, EvalEnv& env) {
    checkParam(params, 1, 1);
    return std::make_shared<BooleanValue>(params[0]->asNumber() == 0);
}
