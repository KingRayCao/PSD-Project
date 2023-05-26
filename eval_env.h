#ifndef EVAL_ENV_H
#define EVAL_ENV_H
#include "value.h"
#include <unordered_map>
class EvalEnv {
private:
    std::unordered_map<std::string, ValuePtr> symbolTable{};

public:
    EvalEnv();
    std::deque<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::deque<ValuePtr> args);
    ValuePtr eval(ValuePtr expr);
    bool add(std::string, ValuePtr);
};
#endif
