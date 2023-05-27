#include <iostream>
#include <string>

#include "tokenizer.h"
#include "value.h"
#include "parser.h"
#include "eval_env.h"
int main() {
    auto env = EvalEnv::createGlobal();
    while (true) {
        try {
            std::cout << ">>> " ;
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                std::exit(0);
            }
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = env->eval(std::move(value));
            //for (auto& token : tokens) {
            //    std::cout << *token << std::endl;
            //}
            std::cout << result->toString() << std::endl;
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
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
//    RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv6);
//    // [...]
//}
