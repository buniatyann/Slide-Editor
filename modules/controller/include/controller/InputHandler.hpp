#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "interfaces/IInputStream.hpp"
#include "controller/parser/CommandParser.hpp"
#include "controller/parser/Token.hpp"
#include <memory>
#include <string>
#include <optional>

namespace slideEditor::controller {

/**
 * InputHandler - Manages input stream and provides high-level input operations
 * 
 * Responsibilities:
 * - Read input from stream
 * - Handle multi-line input (if needed)
 * - Provide input validation
 * - Buffer management for interactive mode
 * - EOF and error handling
 */
class InputHandler {
public:
    explicit InputHandler(core::IInputStream* input);
    /**
     * Reads a complete command line from input stream
     * Returns nullopt on EOF or error
     */
    std::optional<std::string> readCommandLine();
    
    /**
     * Reads a single character from input
     */
    std::optional<char> readChar();
    
    /**
     * Peeks at next character without consuming
     */
    std::optional<char> peekChar();
    bool hasMoreInput() const;
    bool isEOF() const;
    bool hasError() const;
    std::string getErrorMessage() const;
    size_t getCurrentLine() const;
    void reset();
    void clearError();
    
private:
    core::IInputStream* input_;
    
    bool eofReached_;
    bool errorOccurred_;
    std::string errorMessage_;
    size_t lineNumber_; // current line
    
    // Helper methods
    bool skipWhitespace();
    bool isEndOfCommand(char c) const;
};

} // namespace slideEditor::controller

#endif // INPUT_HANDLER_HPP