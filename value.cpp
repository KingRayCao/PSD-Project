#include "value.h"
#include <cmath>
#include <iomanip>
#include <typeinfo>
#include <sstream>
#include "error.h"
#include "eval_env.h"
bool Value::isSelfEvaluating() const {
    return false;
}
bool Value::isNil() const {
    return false;
}
std::optional<std::string> Value::asSymbol() const {
    return std::nullopt;
}
std::deque<ValuePtr> Value::toDeque() const {
    throw LispError("Can't convert a non-pair value into a list.");
    return {};
}
bool Value::isNumber() const {
    return false;
}
bool Value::isList() const {
    return false;
}
double Value::asNumber() const {
    throw LispError("Can't convert a non-numeric value into a number.");
    return 0;
}
std::string Value::asString() const {
    throw LispError("Can't convert a non-string value into a string.");
    return 0;
}
bool Value::operator==(const Value& v) const {
    return false;
}

string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}
bool BooleanValue::isSelfEvaluating() const {
    return true;
}
bool BooleanValue::getValue() const {
    return value;
}
bool BooleanValue::operator==(const Value& v) const {
    if (typeid(v) == typeid(BooleanValue))
        return value == static_cast<const BooleanValue&>(v).value;
    return false;
}

string NumericValue::toString() const {
    if (floor(value) == ceil(value))
        return std::to_string(int(value));
    else
        return std::to_string(value);
}
bool NumericValue::isSelfEvaluating() const {
    return true;
}
bool NumericValue::isNumber() const {
    return true;
}
double NumericValue::asNumber() const {
    return value;
}
bool NumericValue::operator==(const Value& v) const {
    if (typeid(v) == typeid(NumericValue))
        return value == static_cast<const NumericValue&>(v).value;
    return false;
}

string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(value);
    return ss.str();
}
bool StringValue::isSelfEvaluating() const {
    return true;
}
bool StringValue::operator==(const Value& v) const {
    if (typeid(v) == typeid(StringValue))
        return value == static_cast<const StringValue&>(v).value;
    return false;
}
std::string StringValue::asString() const {
    return value;
}

string NilValue::toString() const {
    return "()";
}
bool NilValue::isNil() const {
    return true;
}
bool NilValue::operator==(const Value& v) const {
    if (typeid(v) == typeid(NilValue))
        return true;
    return false;
}
std::deque<std::shared_ptr<Value>> NilValue::toDeque() const {
    return {};
}

string SymbolValue::toString() const {
    return value;
}
std::optional<std::string> SymbolValue::asSymbol() const {
    return {value};
}
bool SymbolValue::operator==(const Value& v) const {
    if (typeid(v) == typeid(SymbolValue))
        return value == static_cast<const SymbolValue&>(v).value;
    return false;
}

PairValue::PairValue(std::deque<ValuePtr> values) {
    if (values.size()==0)
        throw LispError("Can't convert a NilValue into a Pair.");
    right = std::make_shared<NilValue>();
    left = values.back();
    values.pop_back();
    while (!values.empty()) {
        right = std::make_shared<PairValue>(left, right);
        left = values.back();
        values.pop_back();
    }

}
string PairValue::toString() const {
    string ret = "";
    ret += left->toString();
    if (typeid(*right) == typeid(PairValue)) {
        string tmp = right->toString();
        ret += " " + tmp.substr(1, tmp.length() - 2);
    }
    else if (typeid(*right) != typeid(NilValue))
        ret += " . " + right->toString();
    return "(" + ret + ")";
}
std::deque<ValuePtr> PairValue::toDeque() const {
    if (typeid(*right) == typeid(PairValue)) {
        auto ret = right->toDeque();
        ret.push_front(left);
        return ret;
    } 
    else if (typeid(*right) != typeid(NilValue))
        throw(LispError("The pair is not a list."));
    else
        return std::deque<ValuePtr>{left};

}
bool PairValue::isList() const {
    if (typeid(*right) == typeid(PairValue))
        return right->isList();
    else if (typeid(*right) != typeid(NilValue))
        return false;
    else
        return true;
}
ValuePtr PairValue::getCar() const {
    return left;
}
ValuePtr PairValue::getCdr() const {
    return right;
}

std::string BuiltinProcValue::toString() const {
    return "#<procedure>";
}
bool BuiltinProcValue::isSelfEvaluating() const {
    return true;
}
ValuePtr BuiltinProcValue::apply(const std::deque<ValuePtr>& args, EvalEnv& env) {
    return func(args, env);
}

LambdaValue::LambdaValue(const std::deque<ValuePtr>& _params,
                         const std::deque<ValuePtr>& _body,
                         const std::shared_ptr<EvalEnv>& _env)
    : body{_body}, env{_env} {
    for (auto& _param : _params) {
        if (auto param = _param->asSymbol()) 
            params.push_back(*param);
        else
            throw LispError("Unimplemented");
    }
}
string LambdaValue::toString() const {
    return "#<procedure>";
}
ValuePtr LambdaValue::apply(const std::deque<ValuePtr>& args) {
    auto lambda_env = env->createChild(params, args);
    ValuePtr ret;
    for (auto& expr: body) {
        ret = lambda_env->eval(expr);
    }
    return ret;
}
std::ostream& operator<<(std::ostream& ost, Value& v) {
    ost << v.toString();
    return ost;
}
