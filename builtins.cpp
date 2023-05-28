#include "builtins.h"
#include "error.h"
const std::unordered_map<std::string, BuiltinFuncType> BUILTIN_FUNCS{
    //核心库
    {"apply", applyFunc},
    {"display", displayFunc},
    {"displayln", displaylnFunc},
    {"error", errorFunc},
    {"eval", evalFunc},
    {"exit", exitFunc}, 
    {"newline", newlineFunc},
    {"print", printFunc}, 

    //类型检查库
    {"atom?", typeCheckFunc([](ValuePtr v) {
         return (v->isSelfEvaluating() &&
                 typeid(*v) != typeid(BuiltinProcValue)) ||
                typeid(*v) == typeid(NilValue) ||
                typeid(*v) == typeid(SymbolValue);
     })},
    {"boolean?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(BooleanValue);
     })},
    {"integer?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(NumericValue) &&
                floor(v->asNumber()) == ceil(v->asNumber());
     })},
    {"list?", typeCheckFunc([](ValuePtr v) {
         return (typeid(*v) == typeid(PairValue) && v->isList()) || (v->isNil());
     })},
    {"number?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(NumericValue);
     })},
    {"null?",
     typeCheckFunc([](ValuePtr v) { return typeid(*v) == typeid(NilValue); })},
    {"pair?",
     typeCheckFunc([](ValuePtr v) { return typeid(*v) == typeid(PairValue); })},
    {"procedure?", typeCheckFunc([](ValuePtr v) {
         return (typeid(*v) == typeid(BuiltinProcValue) ||
                 typeid(*v) == typeid(LambdaValue));
     })},
    {"string?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(StringValue);
     })},
    {"symbol?", typeCheckFunc([](ValuePtr v) {
         return typeid(*v) == typeid(SymbolValue);
     })},

    // 对子与列表操作库
    {"append", appendFunc},
    {"car", carFunc},
    {"cdr", cdrFunc},
    {"cons", consFunc},
    {"length", lengthFunc},
    {"list", listFunc},
    {"map", mapFunc},
    {"filter", filterFunc},
    {"reduce", reduceFunc},
     
    // 算术运算库
    {"+", addFunc},
    {"-", minusFunc},
    {"*", multiplyFunc},
    {"/", divideFunc},
    {"abs", absFunc}, 
    {"expt", exptFunc}, 
    {"quotient", quotientFunc}, 
    {"modulo", moduloFunc}, 
    {"remainder", remainderFunc}, 

    //比较库
    {"eq?", eqFunc},
    {"equal?", equalFunc}, 
    {"not", notFunc}, 
    {"=", optEqFunc},
    {"<", ltFunc}, 
    {">", gtFunc},
    {"<=", lteFunc},
    {">=", gteFunc},
    {"even?", evenFunc},
    {"odd?", oddFunc},
    {"zero?", zeroFunc}
    };
