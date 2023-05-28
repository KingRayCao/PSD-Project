#include <iostream>
#include <fstream>
#include <string>
#include "tokenizer.h"
#include "value.h"
#include "parser.h"
#include "eval_env.h"
inline bool eq_s(const char* s1, const char* s2) {
    return !strcmp(s1, s2);
}
int main(int argc, char** argv) {
    auto env = EvalEnv::createGlobal();
    std::istream* ist{&std::cin};
    std::ostream* ost{&std::cout};
    if (argc == 5) {
        if ((eq_s(argv[1], "--input") || eq_s(argv[1], "-i")) &&
            (eq_s(argv[3], "--output") || eq_s(argv[3], "-o"))) {
            ist = new std::ifstream(argv[2], std::ios::in);
            ost = new std::ofstream(argv[4], std::ios::out);
        } else {
            std::cerr << "Input Syntax Error." << std::endl;
            std::exit(1);
        }
    }
    else if(argc != 1) {
        std::cerr << "Input Syntax Error." << std::endl;
        std::exit(1);
    }
    while (true) {
        try {
            if (argc == 1)
                *ost << ">>> " ;
            std::string line;
            std::getline(*ist, line);
            if (std::cin.eof()) {
                if (argc == 5) {
                    delete ist;
                    delete ost;
                }
                std::exit(0);
            }
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = env->eval(std::move(value));
            *ost << result->toString() << std::endl;
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            if (argc == 5) {
                delete ist;
                delete ost;
                std::exit(0);
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
