#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#ifndef NOMINMAX
#define NOMINMAX
#endif  // !NOMINMAX
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
#include "render.h"
//int main(int argc, char** argv) {
//    SetConsoleOutputCP(CP_UTF8);
//    std::istream* ist{&std::cin};
//    std::ifstream ifs;
//    //check args and open file
//    if (argc == 3) {
//        if ((eq_s(argv[1], "--input") || eq_s(argv[1], "-i"))) {
//            ifs.open(argv[2], std::ios::in);
//            if (!ifs.is_open()) {
//                std::cerr << "Error: Can't open the file '" << argv[2]
//                          << "'." << std::endl;
//                std::exit(1);
//            }
//            ist = &ifs;
//        } else {
//            std::cerr << "Error: Input Syntax Error." << std::endl;
//            std::exit(1);
//        }
//    }
//    else if(argc != 1) {
//        std::cerr << "Input Syntax Error." << std::endl;
//        std::exit(1);
//    }
//    //main loop
//    std::stack<bool> checkBracket;
//    std::string expr;
//    auto env = EvalEnv::createGlobal();
//    Render render{env};
//    std::string input;//only for filemode
//    if (argc == 1) std::cout << ">>> ";
//    while (true) {
//        try {
//            //repl mode
//            if (argc == 1){
//                char ch = _getch();
//                if (ch == 13) {//end of line
//                    render.render(render.input, 0);
//                    std::cout << '\n';
//                    expr = expr + render.input + " ";
//                    for (auto& c : render.input) {
//                        if (c == '(')
//                            checkBracket.push(true);
//                        else if (c == ')')
//                            if (checkBracket.empty())
//                                throw LispError("Mismatched Parentheses.");//fix
//                            else
//                                checkBracket.pop();
//                    }
//                    //evaluate expression
//                    if (checkBracket.empty()) {
//                        auto tokens = Tokenizer::tokenize(expr);
//                        expr = "";
//                        Parser parser(std::move(tokens));
//                        auto value = parser.parse();
//                        auto result = env->eval(std::move(value));
//                        std::cout << result->toString() << std::endl << ">>> ";
//                    } else {
//                        std::cout << "...\t";
//                    }
//                    render.erase();
//                } else if (ch == '\b') {//backspace
//                    if (render.tailLen < render.input.length()) {
//                        int len0 = render.input.length() - render.tailLen - 1;
//                        render.render(render.input.substr(0, len0) +
//                                          render.input.substr(len0 + 1, render.tailLen),
//                                      render.tailLen);
//                    }
//                } else if (int(ch) == 26) {// ctrl z
//                    std::exit(0);
//                } else if (int(ch) == -32) {//arrow 75 left 77 right
//                    ch = _getch();
//                    if (int(ch) == 75 && render.tailLen < render.input.length())
//                        render.render(render.input, render.tailLen + 1);
//                    if (int(ch) == 77 && render.tailLen > 0)
//                        render.render(render.input, render.tailLen - 1);
//                    //To be implemented
//                } else {
//                    int len0 = render.input.length() - render.tailLen;
//                    render.render(render.input.substr(0, len0) +
//                                      std::string{ch} +
//                                      render.input.substr(len0, render.tailLen),
//                                  render.tailLen);
//                }
//            }
//            //file mode
//            else {
//                std::string line;
//                if (ist->eof()) {
//                    if (!checkBracket.empty()) {
//                        throw LispError("Mismatched Parentheses.");
//                    } else
//                        std::exit(0);
//                }
//                std::getline(*ist, line);
//                input += line;
//                if (!input.empty()) {
//                    for (auto& c : line) {
//                        if (c == '(')
//                            checkBracket.push(true);
//                        else if (c == ')')
//                            if (checkBracket.empty())
//                                throw LispError("Mismatched Parentheses.");
//                            else
//                                checkBracket.pop();
//                    }
//                    if (checkBracket.empty()) {
//                        auto tokens = Tokenizer::tokenize(input);
//                        input = "";
//                        Parser parser(std::move(tokens));
//                        auto value = parser.parse();
//                        auto result = env->eval(std::move(value));
//                    } else {
//                        input += " ";
//                    }
//                }
//            }
//
//        } catch (std::runtime_error& e) {
//            std::cerr << "Error: " << e.what() << std::endl;
//            if (argc != 1)
//                std::exit(1);
//            else {
//                render.erase();
//                std::cout << ">>> ";
//            }
//        }
//    }
//}

#include "rjsj_test.hpp"
struct TestCtx {
    std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(std::move(value));
        return result->toString();
    }
};
int main() {
    RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7, Lv7Lib, Sicp);
    // [...]
}
