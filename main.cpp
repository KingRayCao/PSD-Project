#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#define TokenType Win32TokenType
#include <windows.h>
#undef TokenType
#include <conio.h>
#include "error.h"
#include "tokenizer.h"
#include "value.h"
#include "parser.h"
#include "eval_env.h"
#include "forms.h"
inline bool eq_s(const char* s1, const char* s2) {
    return !strcmp(s1, s2);
}
/*
0 = 黑色 8 = 灰色
1 = 蓝色 9 = 淡蓝色
2 = 绿色 10 = 淡绿色
3 = 浅绿色 11 = 淡浅绿色
4 = 红色 12 = 淡红色
5 = 紫色 13 = 淡紫色
6 = 黄色 14 = 淡黄色
7 = 白色 15 = 亮白色
*/
template <class T>
void colorPrint(const T& c, int color = 7) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
    std::cout << c;
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}
template <class T>
void printTimes(const T& c, int n = 1) {
    for (int i = 0; i < n; ++i) std::cout << c;
}
bool checkTail(const std::string& s1, const std::string& s2) {
    if (s1.size() < s2.size())
        return false;
    for (int i = 1; i <= s2.size(); ++i) {
        if (s1[s1.size() - i] != s2[s2.size() - i]) 
            return false;
    }
    return true;

}

bool render(const std::string& out,
            std::shared_ptr<EvalEnv> env, 
            bool append = true ) {  // append true为键入 false为删除
    int i = out.length() - 1;
    std::string last;
    if (append) {
        if (out[i] == '(' || out[i] == ')')
            colorPrint(out[i], 4);
        else if (out[i] == ' ')
            std::cout << ' ';
        else {
            while (i >= 0 && out[i] != ' ' && out[i] != '(' && out[i] != ')') {
                last = out[i] + last;
                --i;
            }
            printTimes('\b', max(last.length() - 1, 0));
            extern const std::unordered_map<std::string, SpecialFormType>SPECIAL_FORMS;
            if (env->checkSymbol(last) || SPECIAL_FORMS.contains(last))
                colorPrint(last, 2);
            else
                std::cout << last;
        }
    } else {
        while (i >= 0 && out[i] != ' ' && out[i] != '(' && out[i] != ')') {
            last = out[i] + last;
            --i;
        }
        printTimes('\b', last.length() + 1);
        if (env->checkSymbol(last) || SPECIAL_FORMS.contains(last))
            colorPrint(last, 2);
        else
            std::cout << last;
        std::cout << ' ' << '\b';
    }
    return true;
}

int main(int argc, char** argv) {
    SetConsoleOutputCP(CP_UTF8);
    auto env = EvalEnv::createGlobal();
    std::istream* ist{&std::cin};
    std::ifstream ifs;
    if (argc == 3) {
        if ((eq_s(argv[1], "--input") || eq_s(argv[1], "-i"))) {
            ifs.open(argv[2], std::ios::in);
            if (!ifs.is_open()) {
                std::cerr << "Error: Can't open the file '" << argv[2]
                          << "'." << std::endl;
                std::exit(1);
            }
            ist = &ifs;
        } else {
            std::cerr << "Error: Input Syntax Error." << std::endl;
            std::exit(1);
        }
    }
    else if(argc != 1) {
        std::cerr << "Input Syntax Error." << std::endl;
        std::exit(1);
    }
    std::stack<bool> checkBracket;
    std::string input;
    if (argc == 1) std::cout << ">>> ";
    while (true) {
        try {
            if (argc == 1){
                char ch = _getch();
                if (ch == 13) {
                    std::cout << '\n';
                    for (auto& c : input) {
                        if (c == '(')
                            checkBracket.push(true);
                        else if (c == ')')
                            if (checkBracket.empty())
                                throw LispError("Mismatched Parentheses.");//fix
                            else
                                checkBracket.pop();
                    }
                    if (checkBracket.empty()) {
                        auto tokens = Tokenizer::tokenize(input);
                        input = "";
                        Parser parser(std::move(tokens));
                        auto value = parser.parse();
                        auto result = env->eval(std::move(value));
                        std::cout << result->toString() << std::endl << ">>> ";
                    } else {
                        input += " ";
                    }
                } else if (ch == '\b') {
                    if (!input.empty()) input.pop_back();
                    render(input, env, false);
                } else {
                    input.push_back(ch);
                    render(input, env);
                }

                //std::cout << result->toString() << std::endl;
                

            }
            else {
                std::string line;
                if (ist->eof()) {
                    if (!checkBracket.empty()) {
                        throw LispError("Mismatched Parentheses.");
                    } else
                        std::exit(0);
                }
                std::getline(*ist, line);
                input += line;
                if (!input.empty()) {
                    for (auto& c : line) {
                        if (c == '(')
                            checkBracket.push(true);
                        else if (c == ')')
                            if (checkBracket.empty())
                                throw LispError("Mismatched Parentheses.");
                            else
                                checkBracket.pop();
                    }
                    if (checkBracket.empty()) {
                        auto tokens = Tokenizer::tokenize(input);
                        input = "";
                        Parser parser(std::move(tokens));
                        auto value = parser.parse();
                        auto result = env->eval(std::move(value));
                    } else {
                        input += " ";
                    }
                }
            }

        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            if (argc != 1) {
                std::exit(1);
            }
        }
    }
}

//#include "rjsj_test.hpp"
//struct TestCtx {
//    std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
//    std::string eval(std::string input) {
//        auto tokens = Tokenizer::tokenize(input);
//        Parser parser(std::move(tokens));
//        auto value = parser.parse();
//        auto result = env->eval(std::move(value));
//        return result->toString();
//    }
//};
//int main() {
//    RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7, Lv7Lib, Sicp);
//    // [...]
//}
