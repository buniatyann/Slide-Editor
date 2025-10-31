#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"
#include "LexerState.hpp"
#include "interfaces/IInputStream.hpp"
#include <memory>
#include <string>
#include <vector>

namespace slideEditor::controller {

class Lexer {
public:
    explicit Lexer(core::IInputStream* input);
    Token nextToken();
    // Peek at next token without consuming
    Token peekToken();
    std::vector<Token> tokenizeAll();
    size_t getLine() const;
    size_t getColumn() const;

private:
    core::IInputStream* input_;
    LexerStateMachine stateMachine_;
    size_t line_;
    size_t column_;
    std::string buffer_;  // Accumulates characters for current token
    
    void advance();
    char peekChar();
    void consumeChar();
    void clearBuffer();
    
    // Token creation
    Token createIdentifierToken();
    Token createNumberToken();
    Token createEOLToken();
    Token createEOFToken();
    Token createErrorToken(const std::string& message);
    
    void skipWhitespace();
    bool isCommandKeyword(const std::string& word) const;
};

} // namespace slideEditor::controller

#endif