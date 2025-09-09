#include "../../include/parser/Parser.hpp"
#include "../../include/commands/Commands.hpp"
#include <iostream>
#include <map>

Parser::Parser(SlideFactory& slideFactory, ISlideRepository& repo, JsonSerializer& serializer)
    : slideFactory_(slideFactory), repo_(repo), serializer_(serializer) {}

void Parser::displayHelp(const std::string& command) const {
    if (command.empty()) {
        std::cout << "Available commands:\n"
                  << "  create <title> <content> <theme> - Create a new slide\n"
                  << "  addtext <id> <text> --size <float> [--font <font>] [--color <color>] [--line-width <float>] - Add text to a slide\n"
                  << "  addshape <id> <type> <scale> - Add a shape to a slide\n"
                  << "  removetext <id> <index> - Remove text from a slide\n"
                  << "  removeshape <id> <index> - Remove a shape from a slide\n"
                  << "  save <file> - Save presentation to file\n"
                  << "  load <file> - Load presentation from file\n"
                  << "  display - Display all slides\n"
                  << "  help [command] - Show this help or command details\n"
                  << "  exit - Exit the program\n"
                  << "Note: For simplicity, assume no spaces in arguments. Use 'help <command>' for details.\n";
    } 
    else if (command == "create") {
        std::cout << "create <title> <content> <theme>\n"
                  << "  Creates a new slide with the specified title, content, and theme.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "addtext") {
        std::cout << "addtext <id> <text> --size <float> [--font <font>] [--color <color>] [--line-width <float>]\n"
                  << "  Adds text to the slide with ID <id>.\n"
                  << "  Flags:\n"
                  << "    --size <float>   : Required. Sets text size (e.g., 1.5).\n"
                  << "    --font <font>    : Optional. Sets font (e.g., Arial). Default: Arial.\n"
                  << "    --color <color>  : Optional. Sets color (e.g., Red). Default: Black.\n"
                  << "    --line-width <float> : Optional. Sets line width (e.g., 2.0). Default: 1.0.\n";
    } 
    else if (command == "addshape") {
        std::cout << "addshape <id> <type> <scale>\n"
                  << "  Adds a shape (Triangle, Circle, Rectangle, Ellipse) to the slide with ID <id>.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "removetext") {
        std::cout << "removetext <id> <index>\n"
                  << "  Removes text at <index> from the slide with ID <id>.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "removeshape") {
        std::cout << "removeshape <id> <index>\n"
                  << "  Removes shape at <index> from the slide with ID <id>.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "save") {
        std::cout << "save <file>\n"
                  << "  Saves the presentation to <file> in JSON format.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "load") {
        std::cout << "load <file>\n"
                  << "  Loads the presentation from <file>.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "display") {
        std::cout << "display\n"
                  << "  Displays all slides.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "help") {
        std::cout << "help [command]\n"
                  << "  Shows all commands or details for a specific command.\n"
                  << "  No flags available.\n";
    } 
    else {
        std::cout << "Unknown command: " << command << ". Use 'help' to list all commands.\n";
    }
}

std::unique_ptr<ICommand> Parser::parse(const std::vector<Token>& tokens) const {
    if (tokens.empty() || tokens[0].type != Token::Type::Word) {
        std::cout << "Invalid command." << std::endl;
        return nullptr;
    }

    std::string cmd = tokens[0].value;
    if (cmd == "create" && tokens.size() >= 4) {
        if (tokens[1].type == Token::Type::Word && tokens[2].type == Token::Type::Word && tokens[3].type == Token::Type::Word) {
            return std::make_unique<CreateSlideCommand>(slideFactory_, repo_, tokens[1].value, tokens[2].value, tokens[3].value);
        }
    } 
    else if (cmd == "addtext" && tokens.size() >= 4) {
        if (tokens[1].type == Token::Type::Number && tokens[2].type == Token::Type::Word && tokens[3].type == Token::Type::Flag && tokens[3].value == "--size" && tokens.size() >= 5 && tokens[4].type == Token::Type::Float) {
            int slideId = std::stoi(tokens[1].value);
            std::string textContent = tokens[2].value;
            float size = std::stof(tokens[4].value);
            std::string font = "Arial";
            std::string color = "Black";
            float lineWidth = 1.0f;

            for (std::size_t i = 5; i < tokens.size(); ++i) {
                if (tokens[i].type == Token::Type::Flag) {
                    if (tokens[i].value == "--font" && i + 1 < tokens.size() && tokens[i + 1].type == Token::Type::Word) {
                        font = tokens[i + 1].value;
                        ++i;
                    } 
                    else if (tokens[i].value == "--color" && i + 1 < tokens.size() && tokens[i + 1].type == Token::Type::Word) {
                        color = tokens[i + 1].value;
                        ++i;
                    } 
                    else if (tokens[i].value == "--line-width" && i + 1 < tokens.size() && tokens[i + 1].type == Token::Type::Float) {
                        lineWidth = std::stof(tokens[i + 1].value);
                        ++i;
                    } 
                    else {
                        std::cout << "Invalid flag or value for addtext." << std::endl;
                        return nullptr;
                    }
                }
            }
    
            return std::make_unique<AddTextCommand>(repo_, slideId, textContent, size, font, color, lineWidth);
        }
    } 
    else if (cmd == "addshape" && tokens.size() >= 4) {
        if (tokens[1].type == Token::Type::Number && tokens[2].type == Token::Type::Word && tokens[3].type == Token::Type::Float) {
            int slideId = std::stoi(tokens[1].value);
            float scale = std::stof(tokens[3].value);
    
            return std::make_unique<AddShapeCommand>(repo_, slideId, tokens[2].value, scale);
        }
    } 
    else if (cmd == "removetext" && tokens.size() >= 3) {
        if (tokens[1].type == Token::Type::Number && tokens[2].type == Token::Type::Number) {
            int slideId = std::stoi(tokens[1].value);
            std::size_t index = std::stoul(tokens[2].value);
    
            return std::make_unique<RemoveTextCommand>(repo_, slideId, index);
        }
    } 
    else if (cmd == "removeshape" && tokens.size() >= 3) {
        if (tokens[1].type == Token::Type::Number && tokens[2].type == Token::Type::Number) {
            int slideId = std::stoi(tokens[1].value);
            std::size_t index = std::stoul(tokens[2].value);
    
            return std::make_unique<RemoveShapeCommand>(repo_, slideId, index);
        }
    } 
    else if (cmd == "save" && tokens.size() >= 2) {
        if (tokens[1].type == Token::Type::Word) {
            return std::make_unique<SaveCommand>(serializer_, repo_, tokens[1].value);
        }
    } 
    else if (cmd == "load" && tokens.size() >= 2) {
        if (tokens[1].type == Token::Type::Word) {
            return std::make_unique<LoadCommand>(serializer_, repo_, slideFactory_, tokens[1].value);
        }
    } 
    else if (cmd == "display" && tokens.size() == 1) {
        return std::make_unique<DisplayCommand>(repo_);
    } 
    else if (cmd == "help") {
        std::string helpCommand = tokens.size() > 1 && tokens[1].type == Token::Type::Word ? tokens[1].value : "";
        displayHelp(helpCommand);
        return nullptr;
    }

    std::cout << "Invalid command or arguments. Use 'help' for assistance." << std::endl;
    return nullptr;
}