#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include "Lexer.hpp"
#include "Token.hpp"
#include <vector>
#include <string>
#include <memory>

namespace slideEditor::controller {

struct ParsedCommand {
    std::string commandName;
    std::vector<std::string> arguments;
    bool isValid;
    std::string errorMessage;
    
    ParsedCommand();
};

class CommandParser {
public:
    explicit CommandParser(core::IInputStream* input);
    ParsedCommand parseCommand();
    std::vector<ParsedCommand> parseAll();

private:
    std::unique_ptr<Lexer> lexer_;
    Token currentToken_;
    
    // Token management
    void advance();
    bool expect(TokenType type);
    bool match(TokenType type);
    
    ParsedCommand parseCreate();
    ParsedCommand parseAddShape();
    ParsedCommand parseRemoveShape();
    ParsedCommand parseSave();
    ParsedCommand parseLoad();
    ParsedCommand parseUndo();
    ParsedCommand parseRedo();
    ParsedCommand parseDisplay();
    ParsedCommand parseHelp();
    ParsedCommand parseExit();
    ParsedCommand parseUndo();  
    ParsedCommand parseRedo();
    
    bool validateArgumentCount(const std::string& cmd, 
                               size_t expected, 
                               size_t actual);
};

} // namespace slideEditor::controller

#endif // COMMAND_PARSER_HPP