#ifndef BUILTINS_H
#define BUILTINS_H
#include "eval_env.h"
extern const std::unordered_map<std::string, BuiltinFuncType> BUILTIN_FUNCS;
ValuePtr addFunc(const std::deque<ValuePtr>& params);
ValuePtr minusFunc(const std::deque<ValuePtr>& params);
ValuePtr multiplyFunc(const std::deque<ValuePtr>& params);
ValuePtr divideFunc(const std::deque<ValuePtr>& params);
ValuePtr printFunc(const std::deque<ValuePtr>& params);
ValuePtr displayFunc(const std::deque<ValuePtr>& params);
ValuePtr newlineFunc(const std::deque<ValuePtr>& params);
ValuePtr exitFunc(const std::deque<ValuePtr>& params);
template <class F>
BuiltinFuncType typeCheckFunc(F func);
ValuePtr carFunc(const std::deque<ValuePtr>& params);
ValuePtr cdrFunc(const std::deque<ValuePtr>& params);
ValuePtr consFunc(const std::deque<ValuePtr>& params);
ValuePtr lengthFunc(const std::deque<ValuePtr>& params);
ValuePtr listFunc(const std::deque<ValuePtr>& params);
ValuePtr appendFunc(const std::deque<ValuePtr>& params);
#endif  // !BUILTINS_H
