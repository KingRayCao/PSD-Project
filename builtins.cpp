#include "builtins.h"
#include "error.h"
//TODO: Add Builtin Funcs
std::unordered_map<std::string, BuiltinFuncType*> builtinFunc(){
    std::unordered_map<std::string, BuiltinFuncType*> ret;
    ret["+"] = &add;
    ret["print"] = &print;
    return ret;
}
ValuePtr add(const std::deque<ValuePtr>& params) {
    double result = 0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}
ValuePtr print(const std::deque<ValuePtr>& params) {
    for (auto v : params) {
        std::cout << v->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
    
}
