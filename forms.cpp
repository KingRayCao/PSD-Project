#include "forms.h"
#include "error.h"
const std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS{
    {"define", defineForm},
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},       
    {"lambda", lambdaForm}};
ValuePtr defineForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    if (auto name = args[0]->asSymbol()) {
        env.defineBinding(*name, env.eval(args[1]));
        return std::make_shared<NilValue>();
    }
    else {
        auto func = static_cast<PairValue&>(*args[0]);
        auto fname = func.getCar()->asSymbol();
        auto params = func.getCdr()->toDeque();
        auto body = args;
        body.pop_front();
        env.defineBinding(*fname, std::make_shared<LambdaValue>(params, body, env.shared_from_this()));
        return std::make_shared<NilValue>();
    }
    throw LispError("Unimplemented");

}
ValuePtr quoteForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    return args[0];
}
ValuePtr ifForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    auto condition = env.eval(args[0]);
    if (typeid(*condition) == typeid(BooleanValue) &&
        static_cast<BooleanValue&>(*condition).getValue() == false) {
        return env.eval(args[2]);
    }
    else {
        return env.eval(args[1]);
    }
}
ValuePtr andForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    for (auto& arg : args) {
        auto condition = env.eval(arg);
        if (typeid(*condition) == typeid(BooleanValue) &&
            static_cast<BooleanValue&>(*condition).getValue() == false) {
            return condition;
        }
    }
    if (args.size() == 0) 
        return std::make_shared<BooleanValue>(true);
    else
        return env.eval(args[args.size() - 1]);
}
ValuePtr orForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    for (auto& arg : args) {
        auto condition = env.eval(arg);
        if (typeid(*condition) == typeid(BooleanValue) &&
            static_cast<BooleanValue&>(*condition).getValue() == false) {
            continue;
        }
        return condition;
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::deque<ValuePtr>& args, EvalEnv& env) {
    auto params = static_cast<PairValue&>(*args[0]).toDeque();
    auto body = args;
    body.pop_front();
    return std::make_shared<LambdaValue>(params, body, env.shared_from_this());
}
