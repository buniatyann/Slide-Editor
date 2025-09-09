#include "../include/CommandFactory.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    SlideFactory slideFactory;
    SlideRepository repo;
    JsonSerializer serializer;
    CommandFactory cmdFactory(slideFactory, repo, serializer);

    if (argc == 1) {
        cmdFactory.displayHelp();
        return 0;
    }

    std::string command;
    if (argc > 1) {
        // Combine argv into a single command string
        for (int i = 1; i < argc; ++i) {
            command += argv[i];
            if (i < argc - 1) command += " ";
        }
    }

    std::cout << "SlideEditor CLI - Commands: create <title> <content> <theme>, addtext <id> <text> --size <float> [--font <font>] [--color <color>] [--line-width <float>], addshape <id> <type> <scale>, removetext <id> <index>, removeshape <id> <index>, save <file>, load <file>, display, help [command], exit" << std::endl;
    std::cout << "Note: For simplicity, assume no spaces in arguments except between tokens. Use 'help' for details." << std::endl;

    if (!command.empty()) {
        auto cmd = cmdFactory.createCommand(command);
        if (cmd) cmd->execute();
        return 0;
    }

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "exit") 
            break;

        auto cmd = cmdFactory.createCommand(command);
        if (cmd) 
            cmd->execute();
    }

    return 0;
}