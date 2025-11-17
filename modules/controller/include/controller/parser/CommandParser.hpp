#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include "Lexer.hpp"
#include "Token.hpp"
#include <vector>
#include <string>
#include <memory>

namespace slideEditor::controller {

class CommandRegistry;  // Forward declare

struct ParsedCommand {
    std::vector<std::string> arguments;
    std::string commandName;
    std::string errorMessage;
    bool isValid;
    
    ParsedCommand();
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
    void advance();
    bool expect(TokenType type);
    bool match(TokenType type);
};

} // namespace slideEditor::controller

#endif // COMMAND_PARSER_HPP