#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include "Lexer.hpp"
#include "Token.hpp"
#include <vector>
#include <string>
#include <memory>

namespace slideEditor::controller {

class CommandRegistry;  // Forward declare

struct ParsedArgument {
    std::variant<int, double, std::string> value;
    std::string type;  // "int", "double", "string"
    
    ParsedArgument() : value(std::string("")), type("string") {}
    
    explicit ParsedArgument(int v) : value(v), type("int") {}
    explicit ParsedArgument(double v) : value(v), type("double") {}
    explicit ParsedArgument(std::string v) : value(std::move(v)), type("string") {}
    
    int asInt() const {
        if (std::holds_alternative<int>(value)) {
            return std::get<int>(value);
        } 
        else if (std::holds_alternative<double>(value)) {
            return static_cast<int>(std::get<double>(value));
        } 
        else {
            return std::stoi(std::get<std::string>(value));
        }
    }
    
    double asDouble() const {
        if (std::holds_alternative<double>(value)) {
            return std::get<double>(value);
        } 
        else if (std::holds_alternative<int>(value)) {
            return static_cast<double>(std::get<int>(value));
        } 
        else {
            return std::stod(std::get<std::string>(value));
        }
    }
    
    std::string asString() const {
        if (std::holds_alternative<std::string>(value)) {
            return std::get<std::string>(value);
        } 
        else if (std::holds_alternative<int>(value)) {
            return std::to_string(std::get<int>(value));
        } 
        else {
            return std::to_string(std::get<double>(value));
        }
    }
};

struct ParsedCommand {
    std::vector<ParsedArgument> arguments;
    std::string commandName;
    std::string errorMessage;
    bool isValid;
    
    ParsedCommand();

    // Helper to get arguments as strings (for backward compatibility)
    std::vector<std::string> getArgumentStrings() const {
        std::vector<std::string> result;
        for (const auto& arg : arguments) {
            result.push_back(arg.asString());
        }
     
        return result;
    }
};

class CommandParser {
public:
    explicit CommandParser(core::IInputStream* input);
    
    void setRegistry(CommandRegistry* registry);  
    ParsedCommand parseCommand();
    std::vector<ParsedCommand> parseAll();

private:
    std::unique_ptr<Lexer> lexer_;
    Token currentToken_;
    CommandRegistry* registry_;  
    
    // Token management
    void advance(); // next meaningful token (Skips EOL)
    bool expect(TokenType type);
    bool match(TokenType type);

    ParsedArgument parseIntArgument(const std::string& argName);
    ParsedArgument parseDoubleArgument(const std::string& argName);
    ParsedArgument parseStringArgument(const std::string& argName);
};

} // namespace slideEditor::controller

#endif // COMMAND_PARSER_HPP