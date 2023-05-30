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
    while (true) {
        try {
            if (argc == 1)
                std::cout << ">>> " ;
            std::string line;
            if (ist->eof()) {
                std::exit(0);
            }
            std::getline(*ist, line);
            if (!line.empty()) {
                auto tokens = Tokenizer::tokenize(line);
                Parser parser(std::move(tokens));
                auto value = parser.parse();
                auto result = env->eval(std::move(value));
                if (argc == 1) std::cout << result->toString() << std::endl;
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
