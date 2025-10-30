#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

struct Token {
    enum class Type {
        Word,
        Number,
        Float,
        Flag,           // --size
        QuoteString,    // "hello world"
        End,
        Error
    };

    Type type;
    std::string value;

    Token(Type t = Type::End, std::string v = "")
        : type(t), value(std::move(v)) {}
};

#endif // TOKEN_HPP