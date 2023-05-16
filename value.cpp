#include "./value.h"
#include <cmath>
#include <iomanip>
#include <typeinfo>

string BooleanValue::toString() {
    return value ? "#t" : "#f";
}

string NumericValue::toString() {
    if (floor(value) == ceil(value))
        return std::to_string(int(value));
    else
        return std::to_string(value);
}

string StringValue::toString() {
    string ret = "\"";
    for (auto c : value) {
        if (c == '\"' || c == '\\') ret += '\\';
        ret += c;
    }
    return ret + "\"";
}

string NilValue::toString() {
    return "()";
}

string SymbolValue::toString() {
    return value;
}

string PairValue::toString() {
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

std::ostream& operator<<(std::ostream& ost, Value& v) {
    ost << v.toString();
    return ost;
}
