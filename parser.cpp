#include "parser.h"
#include "error.h"

Parser::Parser(std::deque<TokenPtr> tokens0) {
    for (auto it = tokens0.begin(); it != tokens0.end(); ++it) {
        tokens.push_back(std::move(*it));
    }
}
ValuePtr Parser::parse() {
    if (tokens.empty())
        //TODO: Specify Error Type
        throw(SyntaxError("Input Syntax Error"));
    auto token = std::move(tokens.front());
    tokens.pop_front();
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    }
    else if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    }
    else if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    }
    else if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    }
    else if (token->getType() == TokenType::LEFT_PAREN) {
        auto value = this->parseTails();
        return value;
    }
    else if (token->getType() == TokenType::QUOTE || token->getType() == TokenType::QUASIQUOTE || token->getType() == TokenType::UNQUOTE) {
        std::deque<ValuePtr> tmp;
        switch (token->getType()) {
        case TokenType::QUOTE: tmp.push_back(std::make_shared<SymbolValue>("quote")); break;
        case TokenType::QUASIQUOTE: tmp.push_back(std::make_shared<SymbolValue>("quasiquote")); break;
        case TokenType::UNQUOTE: tmp.push_back(std::make_shared<SymbolValue>("unquote")); break;
        }
        tmp.push_back(this->parse());
        return toList(tmp);
    }
    else {
        // TODO: To Be Implemented
        throw SyntaxError("Unimplemented");
    }
}

ValuePtr Parser::parseTails() {
    if (tokens.empty())
        throw(SyntaxError("Input Syntax Error"));
    if (tokens.front()->getType() == TokenType::RIGHT_PAREN) {
        tokens.pop_front();
        return std::make_shared<NilValue>();
    }
    auto car = this->parse();
    if (tokens.empty())
        throw(SyntaxError("Input Syntax Error"));
    if (tokens.front()->getType() == TokenType::DOT) {
        tokens.pop_front();
        auto cdr = this->parse();
        if(tokens.empty() || tokens.front()->getType() != TokenType::RIGHT_PAREN)
            throw(SyntaxError("Input Syntax Error"));
        tokens.pop_front();
        return std::make_shared<PairValue>(car, cdr);
    }
    else {
        auto cdr = this->parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
}

ValuePtr toList(std::deque<ValuePtr> list) {
    if (list.empty()) {
        return std::make_shared<NilValue>();
    }
    else {
        auto car = list.front();
        list.pop_front();
        auto cdr = toList(list);
        return std::make_shared<PairValue>(car, cdr);
    }
}
