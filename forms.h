#ifndef FORMS_H
#define FORMS_H
#include "eval_env.h"
#include <functional>
using SpecialFormType = std::function<ValuePtr(const std::deque<ValuePtr>&, EvalEnv&)>;
extern const std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS;
ValuePtr defineForm(const std::deque<ValuePtr>& args, EvalEnv& env);
ValuePtr quoteForm(const std::deque<ValuePtr>& args, EvalEnv& env);
ValuePtr ifForm(const std::deque<ValuePtr>& args, EvalEnv& env);
ValuePtr andForm(const std::deque<ValuePtr>& args, EvalEnv& env);
ValuePtr orForm(const std::deque<ValuePtr>& args, EvalEnv& env);
ValuePtr lambdaForm(const std::deque<ValuePtr>& args, EvalEnv& env);
ValuePtr condForm(const std::deque<ValuePtr>& args, EvalEnv& env);
ValuePtr beginForm(const std::deque<ValuePtr>& args, EvalEnv& env);
ValuePtr letForm(const std::deque<ValuePtr>& args, EvalEnv& env);
ValuePtr quasiquoteForm(const std::deque<ValuePtr>& args, EvalEnv& env);
#endif  // !FORMS_H
