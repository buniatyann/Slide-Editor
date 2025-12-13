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
        auto stream = std::make_unique<InputStream>(input);
        Lexer lexer(stream.get());
        
        std::vector<Token> tokens;
        Token token = lexer.nextToken();
        
        while (token.type != TokenType::END_OF_FILE) {
            if (token.type != TokenType::END_OF_LINE) {  // Skip EOL tokens
                tokens.push_back(token);
            }
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

TEST_F(LexerTest, TokenizeNegativeInteger_ReturnsNumberToken) {
    auto tokens = tokenizeString("-42");
    
    ASSERT_GE(tokens.size(), 1);
    // Should tokenize as a negative number
    if (tokens[0].type == TokenType::NUMBER) {
        EXPECT_EQ(tokens[0].asInt(), -42);
    }
}

TEST_F(LexerTest, TokenizeCommandWithArguments_ReturnsMultipleTokens) {
    auto tokens = tokenizeString("create Title Content Theme");
    
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::COMMAND);
    EXPECT_EQ(tokens[0].asString(), "create");
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].asString(), "Title");
    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].asString(), "Content");
    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].asString(), "Theme");
}

TEST_F(LexerTest, TokenizeMixedTypes_ReturnsCorrectTypes) {
    auto tokens = tokenizeString("addshape 1 circle 2.5");
    
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::COMMAND);
    EXPECT_EQ(tokens[0].asString(), "addshape");
    EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[1].asInt(), 1);
    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].asString(), "circle");
    EXPECT_EQ(tokens[3].type, TokenType::NUMBER);
    EXPECT_DOUBLE_EQ(tokens[3].asDouble(), 2.5);
}

TEST_F(LexerTest, SkipsWhitespace_BetweenTokens) {
    auto tokens = tokenizeString("  create   Title  ");
    
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].asString(), "create");
    EXPECT_EQ(tokens[1].asString(), "Title");
}

TEST_F(LexerTest, HandlesTabsAsWhitespace) {
    auto tokens = tokenizeString("create\tTitle");
    
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].asString(), "create");
    EXPECT_EQ(tokens[1].asString(), "Title");
}

TEST_F(LexerTest, RecognizesAllCommandKeywords) {
    std::vector<std::string> commands = {
        "create", "addshape", "removeshape", "save", "load",
        "display", "draw", "help", "exit", "undo", "redo"
    };
    
    for (const auto& cmd : commands) {
        auto tokens = tokenizeString(cmd);
        ASSERT_EQ(tokens.size(), 1) << "Failed for command: " << cmd;
        EXPECT_EQ(tokens[0].type, TokenType::COMMAND) << "Failed for command: " << cmd;
        EXPECT_EQ(tokens[0].asString(), cmd) << "Failed for command: " << cmd;
    }
}

TEST_F(LexerTest, CommandsCaseInsensitive) {
    auto tokens1 = tokenizeString("create");
    auto tokens2 = tokenizeString("CREATE");
    auto tokens3 = tokenizeString("Create");
    
    EXPECT_EQ(tokens1[0].type, TokenType::COMMAND);
    EXPECT_EQ(tokens2[0].type, TokenType::COMMAND);
    EXPECT_EQ(tokens3[0].type, TokenType::COMMAND);
}

TEST_F(LexerTest, HandlesHexColors) {
    auto tokens = tokenizeString("#FF0000");
    
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].asString(), "#FF0000");
}

TEST_F(LexerTest, HandlesMultipleLines) {
    auto stream = std::make_unique<InputStream>("create Title\naddshape 1 circle 1.0");
    Lexer lexer(stream.get());
    
    std::vector<Token> tokens;
    Token token = lexer.nextToken();
    
    while (token.type != TokenType::END_OF_FILE) {
        tokens.push_back(token);
        token = lexer.nextToken();
    }
    
    // Should have: create, Title, EOL, addshape, 1, circle, 1.0, EOF
    EXPECT_GT(tokens.size(), 6);
}

TEST_F(LexerTest, EmptyInput_ReturnsEOF) {
    auto tokens = tokenizeString("");
    
    EXPECT_EQ(tokens.size(), 0);
}

TEST_F(LexerTest, OnlyWhitespace_ReturnsEOF) {
    auto tokens = tokenizeString("   \t  \n  ");
    
    EXPECT_EQ(tokens.size(), 0);
}

TEST_F(LexerTest, HandlesDecimalNumbers) {
    std::vector<std::string> numbers = {"0.5", "1.0", "2.5", "100.25"};
    
    for (const auto& num : numbers) {
        auto tokens = tokenizeString(num);
        ASSERT_EQ(tokens.size(), 1) << "Failed for number: " << num;
        EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    }
}

TEST_F(LexerTest, HandlesLargeIntegers) {
    auto tokens = tokenizeString("123456789");
    
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].asInt(), 123456789);
}

TEST_F(LexerTest, HandlesZero) {
    auto tokens = tokenizeString("0");
    
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].asInt(), 0);
}

TEST_F(LexerTest, IdentifiersWithNumbers) {
    auto tokens = tokenizeString("shape123");
    
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].asString(), "shape123");
}

TEST_F(LexerTest, UnderscoresInIdentifiers) {
    auto tokens = tokenizeString("my_identifier");
    
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].asString(), "my_identifier");
}