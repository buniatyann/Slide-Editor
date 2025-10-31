#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <variant>

namespace slideEditor::controller {

enum class TokenType {
    COMMAND,        // create, addshape, etc.
    IDENTIFIER,     // strings, shape types
    NUMBER,         // integers and doubles
    END_OF_LINE,    // newline or semicolon
    END_OF_FILE,
    ERROR
};

struct Token {
    Token(TokenType t, std::string v, size_t ln = 0, size_t col = 0);
    Token(TokenType t, int v, size_t ln = 0, size_t col = 0);
    Token(TokenType t, double v, size_t ln = 0, size_t col = 0);
    Token(TokenType t, size_t ln = 0, size_t col = 0);
    std::string asString() const;
    int asInt() const;
    double asDouble() const;
    std::string toString() const;  // For debugging

public:
    TokenType type;
    std::variant<std::string, int, double> value;
    size_t line;
    size_t column;
};

} // namespace slideEditor::controller 

#endif // TOKEN_HPP