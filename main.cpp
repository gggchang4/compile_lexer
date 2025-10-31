#include "lexer.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    string code = "";  // 存储输入代码
    string temp;
    
    // 从标准输入读取代码
    while (getline(cin, temp)) {
        code = code + temp + "\n";
    }
    
    Lexer l(code);
    l.output();  // 输出token流
    
    return 0;
}