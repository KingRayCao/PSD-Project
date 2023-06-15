#include "render.h"
#define TokenType Win32TokenType
#include <windows.h>
#undef TokenType
#include <cmath>
bool isNumber(const std::string& s) {
    for (auto& c : s) {
        if (!std::isdigit(c) && c != '.') return false;
    }
    return true;
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
void colorPrint(const T& c, int color) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
    std::cout << c;
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}
template <class T>
void printTimes(const T& c, int n) {
    for (int i = 0; i < n; ++i) std::cout << c;
}
bool render(const std::string& out, std::shared_ptr<EvalEnv> env,
            bool append) {  // append true为键入 false为删除
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
            extern const std::unordered_map<std::string, SpecialFormType>
                SPECIAL_FORMS;
            if (env->checkSymbol(last) || SPECIAL_FORMS.contains(last))
                colorPrint(last, 2);
            else if (isNumber(last))
                colorPrint(last, 6);
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
        else if (isNumber(last))
            colorPrint(last, 6);
        else
            std::cout << last;
        std::cout << ' ' << '\b';
    }
    return true;
}
