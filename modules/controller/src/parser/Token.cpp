#include "controller/parser/Token.hpp"
#include <sstream>

namespace slideEditor::controller {

Token::Token(TokenType t, std::string v, size_t ln, size_t col)
    : type(t), value(std::move(v)), line(ln), column(col) {}

Token::Token(TokenType t, int v, size_t ln, size_t col)
    : type(t), value(v), line(ln), column(col) {}

Token::Token(TokenType t, double v, size_t ln, size_t col)
    : type(t), value(v), line(ln), column(col) {}

Token::Token(TokenType t, size_t ln, size_t col)
    : type(t), value(std::string()), line(ln), column(col) {}

std::string Token::asString() const {
    return std::get<std::string>(value);
}

int Token::asInt() const {
    if (std::holds_alternative<int>(value)) {
        return std::get<int>(value);
    }

    return static_cast<int>(std::get<double>(value));
}

double Token::asDouble() const {
    if (std::holds_alternative<double>(value)) {
        return std::get<double>(value);
    }

    return static_cast<double>(std::get<int>(value));
}

std::string Token::toString() const {
    std::ostringstream oss;
    oss << "Token(" << static_cast<int>(type) << ", ";
    if (std::holds_alternative<std::string>(value)) {
        oss << "\"" << std::get<std::string>(value) << "\"";
    } 
    else if (std::holds_alternative<int>(value)) {
        oss << std::get<int>(value);
    } 
    else if (std::holds_alternative<double>(value)) {
        oss << std::get<double>(value);
    }
    
    oss << " at " << line << ":" << column << ")";
    return oss.str();
}

} // namespace slideEditor::controller