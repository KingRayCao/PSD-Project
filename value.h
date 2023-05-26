#ifndef VALUE_H
#define VALUE_H


#include <string>
#include <memory>
#include <iostream>
#include <deque>
#include <optional>
#include <functional>
using std::string;

class Value {
public:
    Value(){};
    virtual string toString() const = 0;
    virtual bool isSelfEvaluating() const;
    virtual bool isNil() const;
    virtual bool isNumber() const;
    virtual double asNumber() const;
    virtual std::deque < std::shared_ptr<Value>> toDeque() const;
    virtual std::optional<std::string> asSymbol() const;
};
using ValuePtr = std::shared_ptr<Value>;
class BooleanValue : public Value {
private:
    bool value;

public:
    explicit BooleanValue(const bool& v) : value{v} {};
    virtual string toString() const override;
    virtual bool isSelfEvaluating() const override;
    bool getValue() const;
};

class NumericValue : public Value {
private:
    double value;

public:
    explicit NumericValue(const double& v) : value{v} {};
    virtual string toString() const override;
    virtual bool isSelfEvaluating() const override;
    virtual bool isNumber() const override;
    virtual double asNumber() const override;
};

class StringValue : public Value {
private:
    string value;

public:
    explicit StringValue(const string& v) : value{v} {};
    virtual string toString() const override;
    virtual bool isSelfEvaluating() const override;
};

class NilValue : public Value {
public:
    NilValue(){};
    virtual string toString() const override;
    virtual bool isNil() const override;
};

class SymbolValue : public Value {
private:
    string value;
   
public:
    explicit SymbolValue(const string& v) : value{v} {};
    virtual string toString() const override;
    virtual std::optional<std::string> asSymbol() const override;
};

class PairValue : public Value {
private:
    ValuePtr left;
    ValuePtr right;

public:
    PairValue(const ValuePtr& l, const ValuePtr& r) : left{l}, right{r} {};
    virtual string toString() const override;
    virtual std::deque<ValuePtr> toDeque() const override;
    ValuePtr getCar() const;
    ValuePtr getCdr() const;
};

using BuiltinFuncType = std::function<ValuePtr(const std::deque<ValuePtr> &)>;
class BuiltinProcValue : public Value {
private:
    BuiltinFuncType func{};
public:
    BuiltinProcValue(BuiltinFuncType _func) : func{_func} {};
    virtual string toString() const override;
    virtual bool isSelfEvaluating() const override;
    ValuePtr apply(const std::deque<ValuePtr>& args);
};

class LambdaValue : public Value {
private:
    std::deque<string> params{};
    std::deque<ValuePtr> body;

public:
    LambdaValue(const std::deque<ValuePtr>& _params,
                const std::deque<ValuePtr>& _body);
    virtual string toString() const override;
};

std::ostream& operator<<(std::ostream& ost, Value& v);

#endif
