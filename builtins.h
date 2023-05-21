#ifndef BUILTINS_H
#define BUILTINS_H
#include "eval_env.h"
std::unordered_map<std::string, BuiltinFuncType*> builtinFunc();
ValuePtr add(const std::deque<ValuePtr>& params);
ValuePtr print(const std::deque<ValuePtr>& params);
#endif  // !BUILTINS_H
