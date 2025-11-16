#include "controller/parser/Lexer.hpp"
#include <cctype>
#include <stdexcept>
#include <algorithm>

namespace slideEditor::controller {

Lexer::Lexer(core::IInputStream* input)
    : input_(input), line_(1), column_(1) {
    if (!input_) {
        throw std::invalid_argument("Input stream cannot be null");
    }
}

Token Lexer::nextToken() {
    stateMachine_.reset();
    clearBuffer();
    
    // Skip whitespace (spaces and tabs, but not newlines)
    while (!input_->eof()) {
        auto maybeChar = input_->peek();
        if (!maybeChar.has_value()) {
            break;
        }
        
        char c = maybeChar.value();
        // Handle newlines as token delimiters
        if (LexerStateMachine::isNewline(c)) {
            input_->get();  // Consume newline
            size_t currentLine = line_;
            size_t currentCol = column_;
            ++line_;
            column_ = 1;

            return Token(TokenType::END_OF_LINE, currentLine, currentCol);
        }
        
        if (c == ' ' || c == '\t') {
            input_->get();
            ++column_;
            continue;
        }
        
        break;
    }
    
    if (input_->eof() || !input_->good()) {
        return createEOFToken();
    }
    
    size_t tokenStartLine = line_;
    size_t tokenStartCol = column_;
    while (!input_->eof()) {
        auto maybeChar = input_->peek();
        if (!maybeChar.has_value()) {
            break;
        }
        
        char c = maybeChar.value();
        LexerState nextState = stateMachine_.transition(c);
        // Check if we should stop accumulating
        if (nextState == LexerState::START && !buffer_.empty()) {
            // Token complete, don't consume this character
            break;
        }
        
        if (nextState == LexerState::ERROR_STATE) {
            return createErrorToken("Invalid character");
        }
        
        // Consume character if it's part of current token
        if (nextState == LexerState::IN_IDENTIFIER ||
            nextState == LexerState::IN_NUMBER ||
            nextState == LexerState::IN_DECIMAL) {
            input_->get();
            buffer_ += c;
            ++column_;
        } 
        else if (nextState == LexerState::IN_WHITESPACE) {
            input_->get();
            ++column_;
        } 
        else {
            break;
        }
    }
    
    // Create token based on accumulated buffer
    if (buffer_.empty()) {
        return createEOFToken();
    }
    
    LexerState finalState = stateMachine_.getCurrentState();    
    if (finalState == LexerState::IN_IDENTIFIER) {
        Token tok = createIdentifierToken();
        tok.line = tokenStartLine;
        tok.column = tokenStartCol;

        return tok;
    } 
    else if (finalState == LexerState::IN_NUMBER || 
               finalState == LexerState::IN_DECIMAL) {
        Token tok = createNumberToken();
        tok.line = tokenStartLine;
        tok.column = tokenStartCol;
    
        return tok;
    }
    
    return createErrorToken("Unexpected end of token");
}

Token Lexer::peekToken() {
    // Save current position (simple implementation without full state save)
    // Note: This is a simplified version; full implementation would need
    // to save/restore stream state
    return nextToken();
}

std::vector<Token> Lexer::tokenizeAll() {
    std::vector<Token> tokens;
    while (true) {
        Token tok = nextToken();
        tokens.push_back(tok);
        if (tok.type == TokenType::END_OF_FILE || 
            tok.type == TokenType::ERROR) {
            break;
        }
    }
    
    return tokens;
}

Token Lexer::createIdentifierToken() {
    TokenType type = isCommandKeyword(buffer_) ? TokenType::COMMAND : TokenType::IDENTIFIER;
    return Token(type, buffer_, line_, column_);
}

Token Lexer::createNumberToken() {
    // Check if it's a double or int
    if (buffer_.find('.') != std::string::npos) {
        double value = std::stod(buffer_);
        return Token(TokenType::NUMBER, value, line_, column_);
    } 
    else {
        int value = std::stoi(buffer_);
        return Token(TokenType::NUMBER, value, line_, column_);
    }
}

Token Lexer::createEOLToken() {
    return Token(TokenType::END_OF_LINE, line_, column_);
}

Token Lexer::createEOFToken() {
    return Token(TokenType::END_OF_FILE, line_, column_);
}

Token Lexer::createErrorToken(const std::string& message) {
    return Token(TokenType::ERROR, message, line_, column_);
}

void Lexer::clearBuffer() {
    buffer_.clear();
}

bool Lexer::isCommandKeyword(const std::string& word) const {
    static const std::vector<std::string> keywords = {
        "create", "addshape", "removeshape", "save", 
        "load", "display", "help", "exit", "undo", "redo"
    };
    
    std::string lower = word;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    for (const auto& kw : keywords) {
        if (lower == kw) {
            return true;
        }
    }

    return false;
}

size_t Lexer::getLine() const {
    return line_;
}

size_t Lexer::getColumn() const {
    return column_;
}

} // namespace slideEditor::controller