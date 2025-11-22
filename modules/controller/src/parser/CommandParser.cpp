#include "controller/parser/CommandParser.hpp"
#include "controller/CommandRegistry.hpp"  
#include <algorithm>
#include <stdexcept>

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
    advance(); // initialize currentToken_ with the first meaningful token 
}

void CommandParser::advance() {
    // FIXED: This loop skips over END_OF_LINE tokens to get the next meaningful token
    // The tokens come from lexer_->nextToken() which reads from the input stream
    // The loop continues until we get a token that's not END_OF_LINE
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

ParsedArgument CommandParser::parseIntArgument(const std::string& argName) {
    if (!match(TokenType::NUMBER)) {
        throw std::runtime_error("Expected integer for argument: " + argName);
    }
    
    int value = currentToken_.asInt();
    advance();  // Consume the token
    
    return ParsedArgument(value);
}

ParsedArgument CommandParser::parseDoubleArgument(const std::string& argName) {
    if (!match(TokenType::NUMBER)) {
        throw std::runtime_error("Expected number for argument: " + argName);
    }
    
    double value = currentToken_.asDouble();
    advance();  // Consume the token
    
    return ParsedArgument(value);
}

ParsedArgument CommandParser::parseStringArgument(const std::string& argName) {
    if (!match(TokenType::IDENTIFIER) && !match(TokenType::NUMBER)) {
        throw std::runtime_error("Expected value for argument: " + argName);
    }
    
    std::string value = currentToken_.asString();
    advance();  // Consume the token
    
    return ParsedArgument(value);
}

ParsedCommand CommandParser::parseCommand() {
    ParsedCommand result;
    if (match(TokenType::END_OF_FILE)) {
        result.isValid = false;
        result.errorMessage = "End of input";

        return result;
    }

    if (!match(TokenType::COMMAND)) {
        result.isValid = false;
        result.errorMessage = "Expected command keyword";
        return result;
    }
    
    // Extract command name and normalize to lowercase
    std::string cmdName = currentToken_.asString();
    std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    result.commandName = cmdName;
    
    // FIXED: advance() consumes the command token and moves to the first argument
    // This is necessary because we've finished processing the command token
    // and need to position at the next token for argument parsing
    advance();
    
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
    
    // FIXED: Parse arguments based on metadata, storing typed values
    // Each argument is parsed according to its expected type and stored
    // as the correct type (int, double, or string) in ParsedArgument
    const auto& argInfo = metaCmd->getArgumentInfo();
    try {
        for (size_t i = 0; i < argInfo.size(); ++i) {
            const auto& arg = argInfo[i];
            // Check if we have more tokens
            if (match(TokenType::END_OF_FILE) || match(TokenType::END_OF_LINE)) {
                if (arg.required) {
                    result.isValid = false;
                    result.errorMessage = "Missing required argument: " + arg.name;

                    return result;
                }

                break;  // Optional argument missing, stop parsing
            }
            
            // FIXED: Parse based on expected type and store the actual parsed value
            // Each parse method extracts the typed value and advances the token
            ParsedArgument parsedArg;
            if (arg.type == "int") {
                parsedArg = parseIntArgument(arg.name);
            } 
            else if (arg.type == "double") {
                parsedArg = parseDoubleArgument(arg.name);
            } 
            else {  
                parsedArg = parseStringArgument(arg.name); // string or identifier
            }
            
            result.arguments.push_back(parsedArg);
        }
    } catch (const std::exception& e) {
        result.isValid = false;
        result.errorMessage = e.what();

        return result;
    }
    
    // Validate using metadata (optional additional validation)
    auto argStrings = result.getArgumentStrings();
    if (!metaCmd->validateArguments(argStrings)) {
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