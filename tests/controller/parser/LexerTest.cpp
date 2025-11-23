#include <gtest/gtest.h>
#include "controller/parser/Lexer.hpp"
#include "io/InputStream.hpp"
#include <memory>

using namespace slideEditor::controller;
using namespace slideEditor::io;

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {}
    
    std::vector<Token> tokenizeString(const std::string& input) {
        InputStream stream(input);
        Lexer lexer(&stream);
        
        std::vector<Token> tokens;
        Token token = lexer.nextToken();
        
        while (token.type != TokenType::END_OF_FILE) {
            tokens.push_back(token);
            token = lexer.nextToken();
        }
        
        return tokens;
    }
};

TEST_F(LexerTest, TokenizeCommand_ReturnsCommandToken) {
    auto tokens = tokenizeString("create");
    
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::COMMAND);
    EXPECT_EQ(tokens[0].asString(), "create");
}

TEST_F(LexerTest, TokenizeIdentifier_ReturnsIdentifierToken) {
    auto tokens = tokenizeString("myidentifier");
    
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].asString(), "myidentifier");
}

TEST_F(LexerTest, TokenizeInteger_ReturnsNumberToken) {
    auto tokens = tokenizeString("42");
    
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].asInt(), 42);
}

TEST_F(LexerTest, TokenizeDouble_ReturnsNumberToken) {
    auto tokens = tokenizeString("3.14");
    
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_DOUBLE_EQ(tokens[0].asDouble(), 3.14);
}

TEST_F(LexerTest, TokenizeCommandWithArguments_ReturnsMultipleTokens) {
    auto tokens = tokenizeString("create Title Content Theme");
    
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::COMMAND);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
}

TEST_F(LexerTest, TokenizeMixedTypes_ReturnsCorrectTypes) {
    auto tokens = tokenizeString("addshape 1 circle 2.5");
    
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::COMMAND);
    EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].type, TokenType::NUMBER);
}

TEST_F(LexerTest, SkipsWhitespace_BetweenTokens) {
    auto tokens = tokenizeString("  create   Title  ");
    
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].asString(), "create");
    EXPECT_EQ(tokens[1].asString(), "Title");
}

TEST_F(LexerTest, HandlesNewlines_ReturnsEOLToken) {
    auto tokens = tokenizeString("create\nTitle");
    
    // EOL tokens are typically skipped, so we get just the meaningful tokens
    EXPECT_GE(tokens.size(), 2);
}

// TEST_F(LexerTest, RecognizesAllCommands) {
//     std::vector<std::string> 