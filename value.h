#ifndef VALUE_H
#define VALUE_H


#include <string>
#include <memory>
#include <iostream>
using std::string;

class Value {
public:
    Value(){};
    virtual string toString() = 0;
};
using ValuePtr = std::shared_ptr<Value>;
class BooleanValue : public Value {
private:
    bool value;

public:
    explicit BooleanValue(const bool& v) : value{v} {};
    virtual string toString();
};

class NumericValue : public Value {
private:
    double value;

public:
    explicit NumericValue(const double& v) : value{v} {};
    virtual string toString();
};

class StringValue : public Value {
private:
    string value;

public:
    explicit StringValue(const string& v) : value{v} {};
    virtual string toString();
};

class NilValue : public Value {
public:
    NilValue(){};
    virtual string toString();
};

class SymbolValue : public Value {
private:
    string value;
   
public:
    explicit SymbolValue(const string& v) : value{v} {};
    virtual string toString();
};

class PairValue : public Value {
private:
    ValuePtr left;
    ValuePtr right;

public:
    PairValue(const ValuePtr& l, const ValuePtr& r) : left{l}, right{r} {};
    virtual string toString();
};

std::ostream& operator<<(std::ostream& ost, Value& v);

#endif
