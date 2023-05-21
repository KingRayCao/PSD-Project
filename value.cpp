#include "value.h"
#include <cmath>
#include <iomanip>
#include <typeinfo>
#include <sstream>
#include "error.h"
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
    return {};
}
bool Value::isNumber() const {
    return false;
}
double Value::asNumber() const {
    return 0;
}

string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}
bool BooleanValue::isSelfEvaluating() const {
    return true;
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

string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(value);
    return ss.str();
}
bool StringValue::isSelfEvaluating() const {
    return true;
}

string NilValue::toString() const {
    return "()";
}
bool NilValue::isNil() const {
    return true;
}

string SymbolValue::toString() const {
    return value;
}
std::optional<std::string> SymbolValue::asSymbol() const {
    return {value};
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
        throw(LispError("Convert to deque failed"));
    else
        return std::deque<ValuePtr>{left};

}

std::string BuiltinProcValue::toString() const {
    return "#<procedure>";
}
bool BuiltinProcValue::isSelfEvaluating() const {
    return true;
}
ValuePtr BuiltinProcValue::apply(const std::deque<ValuePtr>& args) {
    return func(args);
}

std::ostream& operator<<(std::ostream& ost, Value& v) {
    ost << v.toString();
    return ost;
}
