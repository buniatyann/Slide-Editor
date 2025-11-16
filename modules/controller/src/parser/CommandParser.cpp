#include "controller/parser/CommandParser.hpp"
#include <algorithm>

namespace slideEditor::controller {

ParsedCommand::ParsedCommand() 
    : isValid(false) {}

CommandParser::CommandParser(core::IInputStream* input)
    : lexer_(std::make_unique<Lexer>(input)),
      currentToken_(TokenType::END_OF_FILE) {
    advance();  // Load first token
}

void CommandParser::advance() {
    do {
        currentToken_ = lexer_->nextToken();
    } while (currentToken_.type == TokenType::END_OF_LINE);  // Skip empty lines
}

bool CommandParser::expect(TokenType type) {
    if (currentToken_.type == type) {
        advance();
        return true;
    }

    return false;
}

bool CommandParser::match(TokenType type) {
    return currentToken_.type == type;
}

ParsedCommand CommandParser::parseCommand() {
    ParsedCommand result;
    if (match(TokenType::END_OF_FILE)) {
        result.isValid = false;
        result.errorMessage = "End of input";

        return result;
    }
    
    // Expect command token
    if (!match(TokenType::COMMAND)) {
        result.isValid = false;
        result.errorMessage = "Expected command keyword";
 
        return result;
    }
    
    std::string cmdName = currentToken_.asString();
    std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    advance();  // Consume command
    
    if (cmdName == "create") {
        return parseCreate();
    } 
    else if (cmdName == "addshape") {
        return parseAddShape();
    } 
    else if (cmdName == "removeshape") {
        return parseRemoveShape();
    } 
    else if (cmdName == "save") {
        return parseSave();
    } 
    else if (cmdName == "load") {
        return parseLoad();
    } 
    else if (cmdName == "display") {
        return parseDisplay();
    } 
    else if (cmdName == "undo") {
        return parseUndo();  
    } 
    else if (cmdName == "redo") {
        return parseRedo();  
    }
    else if (cmdName == "help") {
        return parseHelp();
    } 
    else if (cmdName == "exit") {
        return parseExit();
    }
    
    result.isValid = false;
    result.errorMessage = "Unknown command: " + cmdName;
    
    return result;
}

ParsedCommand CommandParser::parseCreate() {
    ParsedCommand result;
    result.commandName = "create";
    
    // create <title> <content> <theme>
    // Expect 3 arguments
    
    for (int i = 0; i < 3; ++i) {
        if (!match(TokenType::IDENTIFIER)) {
            result.isValid = false;
            result.errorMessage = "Expected argument " + std::to_string(i + 1);
    
            return result;
        }
    
        result.arguments.push_back(currentToken_.asString());
        advance();
    }
    
    result.isValid = true;
    return result;
}

ParsedCommand CommandParser::parseAddShape() {
    ParsedCommand result;
    result.commandName = "addshape";
    
    // addshape <id> <type> <scale>
    
    // Argument 1: slide ID (number)
    if (!match(TokenType::NUMBER)) {
        result.isValid = false;
        result.errorMessage = "Expected slide ID (number)";
    
        return result;
    }
    
    result.arguments.push_back(std::to_string(currentToken_.asInt()));
    advance();
    // Argument 2: shape type (identifier)
    if (!match(TokenType::IDENTIFIER)) {
        result.isValid = false;
        result.errorMessage = "Expected shape type";

        return result;
    }

    result.arguments.push_back(currentToken_.asString());
    advance();
    
    // Argument 3: scale (number)
    if (!match(TokenType::NUMBER)) {
        result.isValid = false;
        result.errorMessage = "Expected scale (number)";

        return result;
    }

    result.arguments.push_back(std::to_string(currentToken_.asDouble()));
    advance();    
    result.isValid = true;

    return result;
}

ParsedCommand CommandParser::parseRemoveShape() {
    ParsedCommand result;
    result.commandName = "removeshape";
    
    // removeshape <id> <index>
    
    // Argument 1: slide ID
    if (!match(TokenType::NUMBER)) {
        result.isValid = false;
        result.errorMessage = "Expected slide ID (number)";
    
        return result;
    }
    result.arguments.push_back(std::to_string(currentToken_.asInt()));
    advance();
    
    // Argument 2: shape index
    if (!match(TokenType::NUMBER)) {
        result.isValid = false;
        result.errorMessage = "Expected shape index (number)";
    
        return result;
    }
    
    result.arguments.push_back(std::to_string(currentToken_.asInt()));
    advance();
    result.isValid = true;

    return result;
}

ParsedCommand CommandParser::parseSave() {
    ParsedCommand result;
    result.commandName = "save";
    
    // save <file>
    if (!match(TokenType::IDENTIFIER)) {
        result.isValid = false;
        result.errorMessage = "Expected filename";
        return result;
    }
    
    result.arguments.push_back(currentToken_.asString());
    advance();
    result.isValid = true;

    return result;
}

ParsedCommand CommandParser::parseLoad() {
    ParsedCommand result;
    result.commandName = "load";
    
    // load <file>
    if (!match(TokenType::IDENTIFIER)) {
        result.isValid = false;
        result.errorMessage = "Expected filename";
    
        return result;
    }
    
    result.arguments.push_back(currentToken_.asString());
    advance();
    result.isValid = true;

    return result;
}

ParsedCommand CommandParser::parseDisplay() {
    ParsedCommand result;
    result.commandName = "display";
    result.isValid = true;
    
    return result;
}

ParsedCommand CommandParser::parseHelp() {
    ParsedCommand result;
    result.commandName = "help";
    
    // help [command] - optional argument
    if (match(TokenType::IDENTIFIER) || match(TokenType::COMMAND)) {
        result.arguments.push_back(currentToken_.asString());
        advance();
    }
    
    result.isValid = true;
    return result;
}

ParsedCommand CommandParser::parseExit() {
    ParsedCommand result;
    result.commandName = "exit";
    result.isValid = true;

    return result;
}

// No arguments
ParsedCommand CommandParser::parseUndo() {
    ParsedCommand result;
    result.commandName = "undo";
    result.isValid = true;

    return result;
}

// No arguments
ParsedCommand CommandParser::parseRedo() {
    ParsedCommand result;
    result.commandName = "redo";
    result.isValid = true;

    return result;
}

std::vector<ParsedCommand> CommandParser::parseAll() {
    std::vector<ParsedCommand> commands;
    while (!match(TokenType::END_OF_FILE)) {
        ParsedCommand cmd = parseCommand();
        commands.push_back(cmd);        
        if (!cmd.isValid && match(TokenType::END_OF_FILE)) {
            break;
        }
    }
    
    return commands;
}

} // namespace slideEditor::controller