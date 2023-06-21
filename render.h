#ifndef  RENDER_H
#define RENDER_H
#include <iostream>
#include <string>
#include <stack>
#include <conio.h>
#include "error.h"
#include "tokenizer.h"
#include "value.h"
#include "parser.h"
#include "eval_env.h"
#include "forms.h"
bool isNumber(const std::string& s);
template <class T>
void colorPrint(const T& c, int color = 7);
template <class T>
void printTimes(const T& c, int n = 1);
void flushLine(int n);
class Render {
private:
    std::shared_ptr<EvalEnv> env;
    void printToken(const std::string& token);

public:
    std::string input{};
    std::deque<std::string> history{""};
    int tailLen{0};
    int historyPos{0};
    Render(const std::shared_ptr<EvalEnv>& _env) : env{_env} {};
    void render(const std::string& rInput, const int& rTailLen);
    void init();
};
#endif  // ! RENDER_H
