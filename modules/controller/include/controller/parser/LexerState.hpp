#ifndef LEXER_STATE_HPP
#define LEXER_STATE_HPP

namespace slideEditor::controller {

enum class LexerState {
    START,              // Initial state
    IN_IDENTIFIER,      // Reading alphanumeric sequence
    IN_NUMBER,          // Reading integer part
    IN_DECIMAL,         // Reading fractional part
    IN_WHITESPACE,      // Skipping spaces/tabs
    ERROR_STATE         // Invalid input
};

class LexerStateMachine {    
public:
    LexerStateMachine();
    // State transitions
    LexerState transition(char c);
    LexerState getCurrentState() const;
    bool isAcceptingState() const;
    bool isErrorState() const;
    void reset();
    static bool isAlpha(char c);
    static bool isDigit(char c);
    static bool isAlphaNum(char c);
    static bool isWhitespace(char c);
    static bool isNewline(char c);

private:
    LexerState currentState_;
    char currentChar_;
};

} // namespace slideEditor::controller

#endif // LEXER_STATE_HPP