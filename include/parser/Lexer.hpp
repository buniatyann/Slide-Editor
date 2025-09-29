#ifndef SLIDEEDITOR_LEXER_HPP
#define SLIDEEDITOR_LEXER_HPP

#include <vector>
#include <string>

struct Token {
    enum class Type { 
        Command, 
        Number, 
        Float, 
        Word, 
        Flag 
    };
    Type type;
    std::string value;

    Token(Type t, const std::string& v) : type(t), value(v) {}
};

class Lexer {
public:
    std::vector<Token> tokenize(const std::vector<std::string>& words) const;
private:
    bool isNumber(const std::string& str) const;
    bool isFloat(const std::string& str) const;
    bool isFlag(const std::string& str) const;
};

#endif // SLIDEEDITOR_LEXER_HPP