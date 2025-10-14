#ifndef SLIDEEDITOR_PARSER_PARSER_HPP
#define SLIDEEDITOR_PARSER_PARSER_HPP

#include "Lexer.hpp"
#include "../interfaces/ICommand.hpp"
#include "../core/SlideFactory.hpp"
#include "../interfaces/ISlideRepository.hpp"
#include "../serialization/JsonSerializer.hpp"
#include <istream>

class Parser {
public:
    Parser(SlideFactory& slideFactory, ISlideRepository& repo, JsonSerializer& serializer);
    std::unique_ptr<ICommand> parse(std::istream& input) const;
    void displayHelp(const std::string& command = "") const;

private:
    enum class State {
        START,
        CREATE_TITLE, CREATE_CONTENT, CREATE_THEME,
        ADDTEXT_ID, ADDTEXT_TEXT, ADDTEXT_SIZE_FLAG, ADDTEXT_SIZE_VALUE,
        ADDTEXT_FONT_FLAG, ADDTEXT_FONT_VALUE,
        ADDTEXT_COLOR_FLAG, ADDTEXT_COLOR_VALUE,
        ADDTEXT_LINEWIDTH_FLAG, ADDTEXT_LINEWIDTH_VALUE,
        ADDSHAPE_ID, ADDSHAPE_TYPE, ADDSHAPE_SCALE,
        MOVE_FROM, MOVE_TO,
        REMOVETEXT_ID, REMOVETEXT_INDEX,
        REMOVESHAPE_ID, REMOVESHAPE_INDEX,
        SAVE_FILE,
        LOAD_FILE,
        DISPLAY,
        HELP, HELP_COMMAND,
        ERROR
    };

    // void displayHelp(const std::string& command = "") const; initially it was here not in public field
    SlideFactory& slideFactory_;
    ISlideRepository& repo_;
    JsonSerializer& serializer_;
};


#endif // SLIDEEDITOR_PARSER_PARSER_HPP