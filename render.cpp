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
void flushLine(int n) {
    printTimes('\b', n);
    printTimes(' ', n);
    printTimes('\b', n);
}
void Render::printToken(const std::string& token) {
    if (!token.empty())
        if (env->checkSymbol(token) || SPECIAL_FORMS.contains(token))
            colorPrint(token, 2);
        else if (isNumber(token))
            colorPrint(token, 1);
        else
            std::cout << token;
}
void Render::render(const std::string& rInput, const int& rTailLen) {
    //flushLine
    printTimes(' ', tailLen);
    flushLine(input.length());
    //render
    std::string token{};
    for (auto& c : rInput) {
        if (c == '(' || c == ')' || c == ' ') {
            printToken(token);
            token = "";
            if (c == ' ')
                std::cout << c;
            else
                colorPrint(c, 4);
        }
        else {
            token += c;
        }
    }
    printToken(token);
    printTimes('\b', rTailLen);
    //update
    input = rInput;
    tailLen = rTailLen;
}
void Render::erase() {
    input = "";
    tailLen = 0;
}
