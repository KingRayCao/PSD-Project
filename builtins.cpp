#include "builtins.h"
#include "error.h"
//TODO: Add Builtin Funcs
const std::unordered_map<std::string, BuiltinFuncType> BUILTIN_FUNCS{
    {"+", addFunc},
    {"-", minusFunc},
    {"*", multiplyFunc},
    {"/", divideFunc}, 
    {"print", printFunc}, 
    {"display", displayFunc},
    {"newline", newlineFunc}, 
    {"exit", exitFunc}, 
    {"atom?", typeCheckFunc([](ValuePtr v) { 
        return v->isSelfEvaluating() || typeid(*v) == typeid(NilValue) ||
                typeid(*v) == typeid(SymbolValue);
        })},
    {"boolean?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(BooleanValue);
     })},
    {"symbol?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(SymbolValue);
     })},
    {"null?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(NilValue);
     })},
    {"number?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(NumericValue);
     })},
    {"string?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(StringValue);
     })},
    {"procedure?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(BuiltinProcValue);//?
     })},
    {"symbol?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(SymbolValue);
     })},
    {"pair?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(PairValue);
     })},
    {"integer?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(NumericValue) &&
                floor(v->asNumber()) == ceil(v->asNumber());
     })},
    {"list?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(PairValue) &&
                v->isList();
     })},
    {"car", carFunc},
    {"cdr", cdrFunc},
    {"cons", consFunc},
    {"length", lengthFunc},
    {"list", listFunc},
    {"append", appendFunc},
    {">", ltFunc}
    };
ValuePtr addFunc(const std::deque<ValuePtr>& params) {
    double result = 0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr minusFunc(const std::deque<ValuePtr>& params) {
    if (params.size() == 1) {
        auto& i = params[0];
        if (!i->isNumber()) {
            throw LispError("Cannot minus a non-numeric value.");
        }
        return std::make_shared<NumericValue>(-i->asNumber());
    } else if (params.size() == 2) {
        auto& x = params[0];
        auto& y = params[1];
        if (!x->isNumber()) {
            throw LispError("Cannot minus a non-numeric value.");
        }
        return std::make_shared<NumericValue>(x->asNumber() - y->asNumber());
    } else {
        throw LispError("Incorrect number of parameters.");
    }
}
ValuePtr multiplyFunc(const std::deque<ValuePtr>& params) {
    double result = 1;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot multiply a non-numeric value.");
        }
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr divideFunc(const std::deque<ValuePtr>& params) {
    if (params.size() == 1) {
        auto& i = params[0];
        if (!i->isNumber()) {
            throw LispError("Cannot divide a non-numeric value.");
        }
        else if (i->asNumber() == 0) {
            throw LispError("Cannot divide by zero.");
        }
        return std::make_shared<NumericValue>(1.0 / i->asNumber());
    } else if (params.size() == 2) {
        auto& x = params[0];
        auto& y = params[1];
        if (!x->isNumber()) {
            throw LispError("Cannot divide a non-numeric value.");
        }
        else if (!y->isNumber()) {
            throw LispError("Cannot divide by a non-numeric value.");
        }
        else if (y->asNumber() == 0) {
            throw LispError("Cannot divide by zero.");
        }
        return std::make_shared<NumericValue>(x->asNumber() / y->asNumber());
    } else {
        throw LispError("Incorrect number of parameters.");
    }
}

ValuePtr printFunc(const std::deque<ValuePtr>& params) {
    for (auto v : params) {
        std::cout << v->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
    
}

ValuePtr newlineFunc(const std::deque<ValuePtr>& params) {
    if (params.size() != 0)
        throw LispError("Incorrect number of parameters.");
    else
        std::cout << std::endl;
    return std::make_shared<NilValue>();
}
ValuePtr displayFunc(const std::deque<ValuePtr>& params) {
    for (auto v : params) {
        if (typeid(*v) == typeid(StringValue)) {
            auto s = v->toString();
            std::cout << s.substr(1, s.size()-2);
        } else
            std::cout << v->toString();
    }
    return std::make_shared<NilValue>();
}

ValuePtr exitFunc(const std::deque<ValuePtr>& params) {
    exit(0);
}
template <class F>
BuiltinFuncType typeCheckFunc(F func) {
    return [&](const std::deque<ValuePtr>& params) { 
        if (params.size() != 1)
            throw LispError("Incorrect number of parameters.");
        else
            return std::make_shared<BooleanValue>(func(params[0]));
    };
}

ValuePtr carFunc(const std::deque<ValuePtr>& params) {
    if (params.size() != 1)
        throw LispError("Incorrect number of parameters.");
    else if (typeid(*params[0]) != typeid(PairValue))
        throw LispError("Cannot apply car to a non-pair value.");
    else
        return static_cast<PairValue&>(*params[0]).getCar();
}
ValuePtr cdrFunc(const std::deque<ValuePtr>& params) {
    if (params.size() != 1)
        throw LispError("Incorrect number of parameters.");
    else if (typeid(*params[0]) != typeid(PairValue))
        throw LispError("Cannot apply cdr to a non-pair value.");
    else
        return static_cast<PairValue&>(*params[0]).getCdr();
}
ValuePtr consFunc(const std::deque<ValuePtr>& params) {
    if (params.size() != 2)
        throw LispError("Incorrect number of parameters.");
    else
        return std::make_shared<PairValue>(params[0], params[1]);
}
ValuePtr lengthFunc(const std::deque<ValuePtr>& params) {
    if (params.size() != 1) 
        throw LispError("Incorrect number of parameters.");
    else if (!params[0]->isList()) {
        throw LispError("Cannot apply length to a non-list value.");
    }
    else {
        return std::make_shared<NumericValue>(params[0]->toDeque().size());
    }
}
ValuePtr listFunc(const std::deque<ValuePtr>& params) {
    if (params.size() == 0) 
        throw LispError("Incorrect number of parameters.");
    else {
        return std::make_shared<PairValue>(params);
    }
}

ValuePtr appendFunc(const std::deque<ValuePtr>& params) {
    if (params.size() == 0) 
        return std::make_shared<NilValue>();
    else {
        std::deque<ValuePtr> lists{};
        for (auto& param: params)
            if (param->isList()) {
                auto list = param->toDeque();
                lists.insert(lists.end(), list.begin(), list.end());
            } else {
                throw LispError("Cannot apply append to a non-list value.");
            }
        return std::make_shared<PairValue>(lists);
    }

}
ValuePtr ltFunc(const std::deque<ValuePtr>& params) {
    return std::make_shared<BooleanValue>(params[0]->asNumber() > params[1]->asNumber());
}
