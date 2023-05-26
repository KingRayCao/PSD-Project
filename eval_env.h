#ifndef EVAL_ENV_H
#define EVAL_ENV_H
#include "value.h"
#include <unordered_map>
class EvalEnv : public std::enable_shared_from_this<EvalEnv> {
private:
    std::unordered_map<std::string, ValuePtr> symbolTable{};
    std::shared_ptr<EvalEnv> parent;
    ValuePtr lookupBinding(const string& name);
    EvalEnv(std::shared_ptr<EvalEnv> _parent);
public:
    static std::shared_ptr<EvalEnv> createGlobal(std::shared_ptr<EvalEnv> _parent = nullptr);
    std::shared_ptr<EvalEnv> createChild(const std::deque<string>& params, const std::deque<ValuePtr>& args);
    std::deque<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::deque<ValuePtr> args);
    ValuePtr eval(ValuePtr expr);
    bool defineBinding(string name, ValuePtr value);
};
#endif
