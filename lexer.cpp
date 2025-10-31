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
    // 读取数字，符合 -?(0|[1-9][0-9]*) 规范
    string num = "";
    
    // 处理负号
    if (getChar() == '-') {
        num += '-';
        next();
    }
    
    // 读取第一位数字
    if (pos >= (int)input.length() || getChar() < '0' || getChar() > '9') {
        // 如果只有负号没有数字，回退并返回负号运算符
        if (num == "-") {
            pos--;
            tokenIndex--;
            return Token(MINUS, "-", tokenIndex++);
        }
    }
    
    char firstDigit = getChar();
    num += firstDigit;
    next();
    
    // 如果第一位是0，则不能有更多数字（不允许前导零）
    if (firstDigit == '0') {
        // 单独的0或-0
        Token t(INTCONST, num, tokenIndex);
        tokenIndex++;
        return t;
    }
    
    // 读取剩余数字（第一位必须是1-9）
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
        
        // 处理负号：如果后面是数字，需要判断是负数还是运算符
        if (c == '-') {
            if (pos + 1 < (int)input.length()) {
                char nextChar = input[pos + 1];
                if (nextChar >= '0' && nextChar <= '9') {
                    // 简化判断：只在明确的情况下认为是负数
                    bool isNegative = false;
                    
                    // 查找前一个非空白字符
                    int prevPos = pos - 1;
                    while (prevPos >= 0) {
                        char pc = input[prevPos];
                        if (pc == ' ' || pc == '\t' || pc == '\n' || pc == '\r' || 
                            pc == '\f' || pc == '\v') {
                            prevPos--;
                        } else {
                            break;
                        }
                    }
                    
                    if (prevPos < 0) {
                        // 前面只有空白或开始位置，是负数
                        isNegative = true;
                    } else {
                        char prevChar = input[prevPos];
                        // 如果前面是左括号、算术运算符、逻辑运算符、分隔符，则是负数
                        if (prevChar == '(' || prevChar == ';' || prevChar == ',' || 
                            prevChar == '{' || prevChar == '+' || prevChar == '*' || 
                            prevChar == '/' || prevChar == '%' || prevChar == '<' || 
                            prevChar == '>' || prevChar == '!' || prevChar == '&' || 
                            prevChar == '|') {
                            isNegative = true;
                        } else if (prevChar == '=') {
                            // = 可能是赋值，需要进一步检查
                            int prev2 = prevPos - 1;
                            while (prev2 >= 0 && (input[prev2] == ' ' || input[prev2] == '\t' || 
                                   input[prev2] == '\n' || input[prev2] == '\r')) {
                                prev2--;
                            }
                            if (prev2 < 0 || input[prev2] == '=' || input[prev2] == '+' ||
                                input[prev2] == '-' || input[prev2] == '*' || input[prev2] == '/' ||
                                input[prev2] == '%' || input[prev2] == '(' || input[prev2] == '<' ||
                                input[prev2] == '>' || input[prev2] == '!' || input[prev2] == '&' ||
                                input[prev2] == '|' || input[prev2] == ';' || input[prev2] == ',' ||
                                input[prev2] == '{') {
                                isNegative = true;
                            }
                        }
                        // 如果前面是字母、数字、下划线、右括号，则-是运算符（不是负数）
                    }
                    
                    if (isNegative) {
                        return readNumber();
                    }
                }
            }
        }
        
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