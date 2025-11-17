#include "controller/parser/CommandParser.hpp"
#include "controller/CommandRegistry.hpp"  
#include <algorithm>

namespace slideEditor::controller {

void CommandParser::setRegistry(CommandRegistry* registry) {
    registry_ = registry;
}

ParsedCommand::ParsedCommand() 
    : isValid(false) {}

CommandParser::CommandParser(core::IInputStream* input)
    : lexer_(std::make_unique<Lexer>(input)),
      currentToken_(TokenType::END_OF_FILE),
      registry_(nullptr) {
    advance();
}

void CommandParser::advance() {
    do {
        currentToken_ = lexer_->nextToken();
    } while (currentToken_.type == TokenType::END_OF_LINE);
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
    
    result.commandName = cmdName;
    advance();  // Consume command
    
    // Get metadata from registry
    if (!registry_) {
        result.isValid = false;
        result.errorMessage = "Command registry not set";

        return result;
    }
    
    const auto* metaCmd = registry_->getMetaCommand(cmdName);
    if (!metaCmd) {
        result.isValid = false;
        result.errorMessage = "Unknown command: " + cmdName;
        return result;
    }
    
    // Parse arguments based on metadata
    const auto& argInfo = metaCmd->getArgumentInfo();
    for (size_t i = 0; i < argInfo.size(); ++i) {
        const auto& arg = argInfo[i];
        // Check if we have more tokens
        if (match(TokenType::END_OF_FILE) || match(TokenType::END_OF_LINE)) {
            if (arg.required) {
                result.isValid = false;
                result.errorMessage = "Missing required argument: " + arg.name;

                return result;
            }

            break;  // Optional argument missing
        }
        
        // Parse based on type
        if (arg.type == "int") {
            if (!match(TokenType::NUMBER)) {
                result.isValid = false;
                result.errorMessage = "Expected number for argument: " + arg.name;
        
                return result;
            }
        
            result.arguments.push_back(std::to_string(currentToken_.asInt()));
        } 
        else if (arg.type == "double") {
            if (!match(TokenType::NUMBER)) {
                result.isValid = false;
                result.errorMessage = "Expected number for argument: " + arg.name;
        
                return result;
            }
        
            result.arguments.push_back(std::to_string(currentToken_.asDouble()));
        } 
        else {  // string or identifier
            if (!match(TokenType::IDENTIFIER) && !match(TokenType::NUMBER)) {
                result.isValid = false;
                result.errorMessage = "Expected value for argument: " + arg.name;
        
                return result;
            }
        
            result.arguments.push_back(currentToken_.asString());
        }
        
        advance();
    }
    
    // Validate using metadata
    if (!metaCmd->validateArguments(result.arguments)) {
        result.isValid = false;
        result.errorMessage = "Invalid arguments for command: " + cmdName;
        
        return result;
    }
    
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