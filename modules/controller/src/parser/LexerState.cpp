#include "controller/parser/LexerState.hpp"
#include <cctype>

namespace slideEditor::controller {

LexerStateMachine::LexerStateMachine() 
    : currentState_(LexerState::START), currentChar_('\0') {}

LexerState LexerStateMachine::transition(char c) {
    currentChar_ = c;
    
    switch (currentState_) {
        case LexerState::START:
            if (isAlpha(c)) {
                currentState_ = LexerState::IN_IDENTIFIER;
            } 
            else if (isDigit(c)) {
                currentState_ = LexerState::IN_NUMBER;
            } 
            else if (isWhitespace(c) && !isNewline(c)) {
                currentState_ = LexerState::IN_WHITESPACE;
            } else if (isNewline(c)) {
                // Stay in START, return END_OF_LINE token
                currentState_ = LexerState::START;
            } 
            else {
                currentState_ = LexerState::ERROR_STATE;
            }
            
            break;
        case LexerState::IN_IDENTIFIER:
            if (isAlphaNum(c)) {
                // Stay in IN_IDENTIFIER
            } 
            else if (isWhitespace(c)) {
                currentState_ = LexerState::START;
            } 
            else if (isNewline(c)) {
                currentState_ = LexerState::START;
            } 
            else {
                currentState_ = LexerState::ERROR_STATE;
            }
            
            break;
            
        case LexerState::IN_NUMBER:
            if (isDigit(c)) {
                // Stay in IN_NUMBER
            } 
            else if (c == '.') {
                currentState_ = LexerState::IN_DECIMAL;
            } 
            else if (isWhitespace(c)) {
                currentState_ = LexerState::START;
            } 
            else if (isNewline(c)) {
                currentState_ = LexerState::START;
            } 
            else {
                currentState_ = LexerState::ERROR_STATE;
            }
            
            break;
            
        case LexerState::IN_DECIMAL:
            if (isDigit(c)) {
                // Stay in IN_DECIMAL
            } 
            else if (isWhitespace(c)) {
                currentState_ = LexerState::START;
            } 
            else if (isNewline(c)) {
                currentState_ = LexerState::START;
            } 
            else {
                currentState_ = LexerState::ERROR_STATE;
            }
            
            break;
            
        case LexerState::IN_WHITESPACE:
            if (isWhitespace(c) && !isNewline(c)) {
                // Stay in IN_WHITESPACE
            } 
            else {
                currentState_ = LexerState::START;
                // Re-process this character in START state
                return transition(c);
            }
            
            break;
            
        case LexerState::ERROR_STATE:
            break; // once in error stay in error
    }
    
    return currentState_;
}

LexerState LexerStateMachine::getCurrentState() const {
    return currentState_;
}

bool LexerStateMachine::isAcceptingState() const {
    return currentState_ == LexerState::IN_IDENTIFIER ||
           currentState_ == LexerState::IN_NUMBER ||
           currentState_ == LexerState::IN_DECIMAL ||
           currentState_ == LexerState::START;
}

bool LexerStateMachine::isErrorState() const {
    return currentState_ == LexerState::ERROR_STATE;
}

void LexerStateMachine::reset() {
    currentState_ = LexerState::START;
    currentChar_ = '\0';
}

bool LexerStateMachine::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool LexerStateMachine::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool LexerStateMachine::isAlphaNum(char c) {
    return isAlpha(c) || isDigit(c);
}

bool LexerStateMachine::isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool LexerStateMachine::isNewline(char c) {
    return c == '\n' || c == '\r';
}

} // namespace slideEditor::controller