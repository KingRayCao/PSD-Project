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
    {"readline", readlineFunc},

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
    {"zero?", zeroFunc},

    //字符串处理
    {"strcat", strcatFunc},
    {"strcmp", strcmpFunc},
    {"stod", stodFunc},
    {"substr", substrFunc},

    //彩蛋
    {"python", easterEggFunc("The Zen of Python, by Tim Peters\n\
Beautiful is better than ugly.\n\
Explicit is better than implicit.\n\
Simple is better than complex.\n\
Complex is better than complicated.\n\
Flat is better than nested.\n\
Sparse is better than dense.\n\
Readability counts.\n\
Special cases aren't special enough to break the rules.\n\
Although practicality beats purity.\n\
Errors should never pass silently.\n\
Unless explicitly silenced.\n\
In the face of ambiguity, refuse the temptation to guess.\n\
There should be one-- and preferably only one --obvious way to do it.\n\
Although that way may not be obvious at first unless you're Dutch.\n\
Now is better than never.\n\
Although never is often better than *right* now.\n\
If the implementation is hard to explain, it's a bad idea.\n\
If the implementation is easy to explain, it may be a good idea.\n\
Namespaces are one honking great idea -- let's do more of those!")},
    {"madoka", easterEggFunc("Don't forget.\n\
Always, somewhere,\n\
someone is fighting for you.\n\
As long as you remember her,\n\
you're not alone.")},
    {"eva", easterEggFunc("👏 Omedetou! 👏\n\
👏 Omedetou! 👏\n\
👏 Omedetou! 👏\n\
👏 Omedetou! 👏\n\
👏 Omedetou! 👏")}
    };
