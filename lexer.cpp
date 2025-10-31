#include "lexer.h"
#include <cctype>
#include <sstream>
using namespace std;

Lexer::Lexer(string s) {
    input = s;
    pos = 0;
    tokenIndex = 0;
    initKeywords();  // 初始化关键字表
}

void Lexer::initKeywords() {
    // 建立关键字映射
    keywordMap["int"] = INT;
    keywordMap["void"] = VOID;
    keywordMap["if"] = IF;
    keywordMap["else"] = ELSE;
    keywordMap["while"] = WHILE;
    keywordMap["break"] = BREAK;
    keywordMap["continue"] = CONTINUE;
    keywordMap["return"] = RETURN;
}

char Lexer::getChar() {
    // 获取当前字符
    if (pos >= (int)input.length()) {
        return '\0';
    }
    return input[pos];
}

char Lexer::peek() {
    // 查看下一个字符
    if (pos + 1 >= (int)input.length()) {
        return '\0';
    }
    return input[pos + 1];
}

void Lexer::next() {
    // 移动到下一个字符
    if (pos < (int)input.length()) {
        pos++;
    }
}

void Lexer::skipSpace() {
    // 跳过空白字符
    while (pos < (int)input.length()) {
        char c = getChar();
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v') {
            next();
        } else {
            break;
        }
    }
}

void Lexer::skipComments() {
    // 跳过注释，支持单行和多行注释
    char c1 = getChar();
    char c2 = peek();
    
    if (c1 == '/' && c2 == '/') {
        // 单行注释
        while (pos < (int)input.length() && getChar() != '\n') {
            next();
        }
    } else if (c1 == '/' && c2 == '*') {
        // 多行注释
        next();
        next();
        while (pos < (int)input.length()) {
            if (getChar() == '*' && peek() == '/') {
                next();
                next();
                break;
            }
            next();
        }
    }
}

Token Lexer::readNumber() {
    // 读取无符号十进制整数，符合 (0|[1-9][0-9]*) 规范
    string num = "";
    
    // 第一位必须是数字
    if (pos >= (int)input.length() || getChar() < '0' || getChar() > '9') {
        // 不应到达这里，由调用方保证
        return Token(UNKNOWN, "", tokenIndex++);
    }
    
    char firstDigit = getChar();
    num += firstDigit;
    next();
    
    // 如果第一位是0，则不能再跟数字（防止前导零）
    if (firstDigit == '0') {
        Token t(INTCONST, num, tokenIndex);
        tokenIndex++;
        return t;
    }
    
    // 读取其余数字
    while (pos < (int)input.length()) {
        char c = getChar();
        if (c >= '0' && c <= '9') {
            num += c;
            next();
        } else {
            break;
        }
    }
    
    Token t(INTCONST, num, tokenIndex);
    tokenIndex++;
    return t;
}

Token Lexer::readId() {
    // 读取标识符或关键字
    string id = "";
    
    while (pos < (int)input.length()) {
        char c = getChar();
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
            (c >= '0' && c <= '9') || c == '_') {
            id += c;
            next();
        } else {
            break;
        }
    }
    
    // 检查是否是关键字
    if (keywordMap.find(id) != keywordMap.end()) {
        Token t(keywordMap[id], id, tokenIndex);
        tokenIndex++;
        return t;
    }
    
    Token t(IDENTIFIER, id, tokenIndex);
    tokenIndex++;
    return t;
}

Token Lexer::readOp() {
    // 读取运算符和分隔符
    char c = getChar();
    Token t;
    
    if (c == '+') {
        next();
        t = Token(PLUS, "+", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '-') {
        next();
        t = Token(MINUS, "-", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '*') {
        next();
        t = Token(MULTIPLY, "*", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '/') {
        next();
        t = Token(DIVIDE, "/", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '%') {
        next();
        t = Token(MODULO, "%", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '=') {
        next();
        if (getChar() == '=') {
            next();
            t = Token(EQUAL, "==", tokenIndex);
            tokenIndex++;
            return t;
        }
        t = Token(ASSIGN, "=", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '!') {
        next();
        if (getChar() == '=') {
            next();
            t = Token(NOT_EQUAL, "!=", tokenIndex);
            tokenIndex++;
            return t;
        }
        t = Token(NOT, "!", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '<') {
        next();
        if (getChar() == '=') {
            next();
            t = Token(LESS_EQUAL, "<=", tokenIndex);
            tokenIndex++;
            return t;
        }
        t = Token(LESS, "<", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '>') {
        next();
        if (getChar() == '=') {
            next();
            t = Token(GREATER_EQUAL, ">=", tokenIndex);
            tokenIndex++;
            return t;
        }
        t = Token(GREATER, ">", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '&') {
        next();
        if (getChar() == '&') {
            next();
            t = Token(AND, "&&", tokenIndex);
            tokenIndex++;
            return t;
        }
        t = Token(UNKNOWN, "&", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '|') {
        next();
        if (getChar() == '|') {
            next();
            t = Token(OR, "||", tokenIndex);
            tokenIndex++;
            return t;
        }
        t = Token(UNKNOWN, "|", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '(') {
        next();
        t = Token(LEFT_PAREN, "(", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == ')') {
        next();
        t = Token(RIGHT_PAREN, ")", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '{') {
        next();
        t = Token(LEFT_BRACE, "{", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == '}') {
        next();
        t = Token(RIGHT_BRACE, "}", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == ';') {
        next();
        t = Token(SEMICOLON, ";", tokenIndex);
        tokenIndex++;
        return t;
    }
    if (c == ',') {
        next();
        t = Token(COMMA, ",", tokenIndex);
        tokenIndex++;
        return t;
    }
    
    next();
    string s = "";
    s += c;
    t = Token(UNKNOWN, s, tokenIndex);
    tokenIndex++;
    return t;
}

Token Lexer::nextToken() {
    // 获取下一个token
    while (pos < (int)input.length()) {
        skipSpace();
        
        if (pos >= (int)input.length()) {
            break;
        }
        
        char c = getChar();
        
        if (c == '/' && (peek() == '/' || peek() == '*')) {
            skipComments();
            continue;
        }
        
        // 处理数字
        if (c >= '0' && c <= '9') {
            return readNumber();
        }
        
        // '-' 始终作为单独的运算符，由 readOp 处理
        // 任何以 '-' 开始的情况都不在这里解析数字
        
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
            return readId();
        }
        
        return readOp();
    }
    
    Token t(END_OF_FILE, "", tokenIndex);
    tokenIndex++;
    return t;
}

vector<Token> Lexer::getAllTokens() {
    // 获取所有token
    vector<Token> result;
    Token t;
    
    do {
        t = nextToken();
        result.push_back(t);
    } while (t.type != END_OF_FILE);
    
    return result;
}

void Lexer::output() {
    // 输出token流
    vector<Token> tokens = getAllTokens();
    
    for (int i = 0; i < (int)tokens.size(); i++) {
        if (tokens[i].type != END_OF_FILE) {
            cout << tokens[i].index << ":" << typeToStr(tokens[i].type) 
                      << ":\"" << tokens[i].value << "\"" << endl;
        }
    }
}

string Lexer::typeToStr(TokenType t) {
    // 将token类型转换为字符串
    if (t == INT) return "'int'";
    if (t == VOID) return "'void'";
    if (t == IF) return "'if'";
    if (t == ELSE) return "'else'";
    if (t == WHILE) return "'while'";
    if (t == BREAK) return "'break'";
    if (t == CONTINUE) return "'continue'";
    if (t == RETURN) return "'return'";
    if (t == IDENTIFIER) return "Ident";
    if (t == INTCONST) return "IntConst";
    if (t == PLUS) return "'+'";
    if (t == MINUS) return "'-'";
    if (t == MULTIPLY) return "'*'";
    if (t == DIVIDE) return "'/'";
    if (t == MODULO) return "'%'";
    if (t == ASSIGN) return "'='";
    if (t == EQUAL) return "'=='";
    if (t == NOT_EQUAL) return "'!='";
    if (t == LESS) return "'<'";
    if (t == LESS_EQUAL) return "'<='";
    if (t == GREATER) return "'>'";
    if (t == GREATER_EQUAL) return "'>='";
    if (t == AND) return "'&&'";
    if (t == OR) return "'||'";
    if (t == NOT) return "'!'";
    if (t == LEFT_PAREN) return "'('";
    if (t == RIGHT_PAREN) return "')'";
    if (t == LEFT_BRACE) return "'{'";
    if (t == RIGHT_BRACE) return "'}'";
    if (t == SEMICOLON) return "';'";
    if (t == COMMA) return "','";
    if (t == END_OF_FILE) return "EOF";
    return "UNKNOWN";
}