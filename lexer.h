#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

enum TokenType {
    INT, VOID, IF, ELSE, WHILE, BREAK, CONTINUE, RETURN,  // 关键字
    IDENTIFIER, INTCONST,  // 标识符和整数
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,  // 算术运算符
    ASSIGN, EQUAL, NOT_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,  // 比较运算符
    AND, OR, NOT,  // 逻辑运算符
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,  // 括号
    SEMICOLON, COMMA,  // 分隔符
    END_OF_FILE, UNKNOWN  // 结束和未知
};

struct Token {
    TokenType type;  // token类型
    string value;    // token值
    int index;       // token序号
    
    Token() {
        type = UNKNOWN;
        value = "";
        index = 0;
    }
    
    Token(TokenType t, string v, int i) {
        type = t;
        value = v;
        index = i;
    }
};

class Lexer {
private:
    string input;      // 输入字符串
    int pos;          // 当前位置
    int tokenIndex;   // token序号
    map<string, TokenType> keywordMap;  // 关键字映射表
    
    void initKeywords();
    char getChar();
    char peek();
    void next();
    void skipSpace();
    void skipComments();
    Token readNumber();
    Token readId();
    Token readOp();
    
public:
    Lexer(string s);
    Token nextToken();
    vector<Token> getAllTokens();
    void output();
    static string typeToStr(TokenType t);
};

#endif
