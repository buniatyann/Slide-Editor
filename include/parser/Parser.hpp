#ifndef SLIDEEDITOR_PARSER_PARSER_HPP
#define SLIDEEDITOR_PARSER_PARSER_HPP

#include "Lexer.hpp"
#include "../interfaces/ICommand.hpp"
#include "../core/SlideFactory.hpp"
#include "../interfaces/ISlideRepository.hpp"
#include "../serialization/JsonSerializer.hpp"

class Parser {
public:
    Parser(SlideFactory& slideFactory, ISlideRepository& repo, JsonSerializer& serializer);
    std::unique_ptr<ICommand> parse(const std::vector<Token>& tokens) const;
    void displayHelp(const std::string& command = "") const;
    
private:
    SlideFactory& slideFactory_;
    ISlideRepository& repo_;
    JsonSerializer& serializer_;
};

#endif // SLIDEEDITOR_PARSER_PARSER_HPP