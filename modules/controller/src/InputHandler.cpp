#include "controller/InputHandler.hpp"
#include <cctype>

namespace slideEditor::controller {

InputHandler::InputHandler(core::IInputStream* input)
    : input_(input), eofReached_(false), errorOccurred_(false),
      lineNumber_(1) {
    if (!input_) {
        errorOccurred_ = true;
        errorMessage_ = "Input stream is null";
    }
}

std::optional<std::string> InputHandler::readCommandLine() {
    if (!input_ || eofReached_) {
        return std::nullopt;
    }
    
    std::string line;
    bool foundContent = false;   
    while (!input_->eof()) {
        auto maybeChar = input_->get();
        if (!maybeChar.has_value()) {
            // EOF reached
            eofReached_ = true;
            break;
        }
        
        char c = maybeChar.value();
        if (c == '\n' || c == '\r') {
            if (c == '\r') {
                auto next = input_->peek();
                if (next.has_value() && next.value() == '\n') {
                    input_->get();  // Consume the \n
                }
            }
            
            lineNumber_++;
            if (foundContent) {
                break;
            }

            continue;
        }
        
        line += c;
        // Mark that we found non-whitespace content
        if (!std::isspace(static_cast<unsigned char>(c))) {
            foundContent = true;
        }
    }
    
    if (foundContent && !line.empty()) {
        return line;
    }
    
    if (eofReached_ && !foundContent) {
        return std::nullopt;
    }
    
    if (line.empty() && !eofReached_) {
        return readCommandLine();  // Recursive call for next line
    }
    
    return foundContent ? std::optional<std::string>(line) : std::nullopt;
}

std::optional<char> InputHandler::readChar() {
    if (!input_ || eofReached_) {
        return std::nullopt;
    }
    
    auto maybeChar = input_->get();
    if (!maybeChar.has_value()) {
        eofReached_ = true;
        return std::nullopt;
    }
    
    char c = maybeChar.value();
    // Track newlines for line numbering
    if (c == '\n') {
        ++lineNumber_;
    }
    
    return c;
}

std::optional<char> InputHandler::peekChar() {
    if (!input_ || eofReached_) {
        return std::nullopt;
    }
    
    return input_->peek();
}

bool InputHandler::skipWhitespace() {
    if (!input_) {
        return false;
    }
    
    while (!input_->eof()) {
        auto maybeChar = input_->peek();    
        if (!maybeChar.has_value()) {
            eofReached_ = true;
            
            return false;
        }
        
        char c = maybeChar.value();
        if (c == '\n' || c == '\r') {
            return true;
        }
        
        if (c == ' ' || c == '\t') {
            input_->get();  // Consume whitespace
            continue;
        }

        return true;
    }
    
    eofReached_ = true;
    return false;
}

bool InputHandler::isEndOfCommand(char c) const {
    return c == '\n' || c == '\r' || c == ';';
}

bool InputHandler::hasMoreInput() const {
    return !eofReached_ && !errorOccurred_;
}

bool InputHandler::isEOF() const {
    return eofReached_;
}

bool InputHandler::hasError() const {
    return errorOccurred_;
}

std::string InputHandler::getErrorMessage() const {
    return errorMessage_;
}

size_t InputHandler::getCurrentLine() const {
    return lineNumber_;
}

void InputHandler::reset() {
    eofReached_ = false;
    errorOccurred_ = false;
    errorMessage_.clear();
    lineNumber_ = 1;
}

void InputHandler::clearError() {
    errorOccurred_ = false;
    errorMessage_.clear();
}

} // namespace slideEditor::controller