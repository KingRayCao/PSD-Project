#ifndef BUILTINS_H
#define BUILTINS_H
#include "eval_env.h"
extern const std::unordered_map<std::string, BuiltinFuncType> BUILTIN_FUNCS;

using BuiltinFunc = ValuePtr(const std::deque<ValuePtr>&, EvalEnv&);
// 工具函数
bool checkParam(const std::deque<ValuePtr>&, int min_n, int max_n=-1);
//核心库
BuiltinFunc applyFunc;
BuiltinFunc displayFunc;
BuiltinFunc displaylnFunc;
BuiltinFunc errorFunc;
BuiltinFunc evalFunc;
BuiltinFunc exitFunc;
BuiltinFunc newlineFunc;
BuiltinFunc printFunc;
BuiltinFunc readlineFunc;

// 类型检查库
template <class F>
BuiltinFuncType typeCheckFunc(F func) {
    return [&](const std::deque<ValuePtr>& params, EvalEnv& env) {
        checkParam(params, 1, 1);
        return std::make_shared<BooleanValue>(func(params[0]));
    };
}

//对子与列表操作库
BuiltinFunc appendFunc;
BuiltinFunc carFunc;
BuiltinFunc cdrFunc;
BuiltinFunc consFunc;
BuiltinFunc lengthFunc;
BuiltinFunc listFunc;
BuiltinFunc mapFunc;
BuiltinFunc filterFunc;
BuiltinFunc reduceFunc;

//算术运算库
BuiltinFunc addFunc;
BuiltinFunc minusFunc;
BuiltinFunc multiplyFunc;
BuiltinFunc divideFunc;
BuiltinFunc absFunc;
BuiltinFunc exptFunc;
BuiltinFunc quotientFunc;
BuiltinFunc moduloFunc;
BuiltinFunc remainderFunc;

//比较库
BuiltinFunc eqFunc;
BuiltinFunc equalFunc;
BuiltinFunc notFunc;
BuiltinFunc optEqFunc;
BuiltinFunc ltFunc;
BuiltinFunc gtFunc;
BuiltinFunc lteFunc;
BuiltinFunc gteFunc;
BuiltinFunc evenFunc;
BuiltinFunc oddFunc;
BuiltinFunc zeroFunc;

//字符串处理
BuiltinFunc strcatFunc;
BuiltinFunc strcmpFunc;
BuiltinFunc stodFunc;
BuiltinFunc substrFunc;

//彩蛋
BuiltinFuncType easterEggFunc(std::string);
#endif  // !BUILTINS_H
