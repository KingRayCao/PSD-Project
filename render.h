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
bool render(const std::string& out, std::shared_ptr<EvalEnv> env,
            bool append = true);


#endif  // ! RENDER_H
