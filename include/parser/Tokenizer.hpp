#ifndef SLIDEEDITOR_PARSER_TOKENIZER_HPP
#define SLIDEEDITOR_PARSER_TOKENIZER_HPP

#include <string>
#include <vector>
#include <istream>

class Tokenizer {
public:
    std::vector<std::string> tokenize(std::istream& input) const;
};

#endif // SLIDEEDITOR_PARSER_TOKENIZER_HPP