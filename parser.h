#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "value.h"
#include <deque>
class Parser {
private:
    std::deque<TokenPtr> tokens;
public:
    Parser(std::deque<TokenPtr>);
    ValuePtr parse();
    ValuePtr parseTails();
};

ValuePtr toList(std::deque<ValuePtr>);
#endif // !PARSER_H
