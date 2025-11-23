#include <gtest/gtest.h>
#include "controller/parser/Token.hpp"

using namespace slideEditor::controller;

class TokenTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(TokenTest, Constructor_String_SetsValues) {
    Token token(TokenType::IDENTIFIER, std::string("test"), 1, 5);
    
    EXPECT_EQ(token.type, TokenType::IDENTIFIER);
    EXPECT_EQ(token.asString(), "test");
    EXPECT_EQ(token.line, 1);
    EXPECT_EQ(token.column, 5);
}

TEST_F(TokenTest, Constructor_Int_SetsValues) {
    Token token(TokenType::NUMBER, 42, 2, 10);
    
    EXPECT_EQ(token.type, TokenType::NUMBER);
    EXPECT_EQ(token.asInt(), 42);
}

TEST_F(TokenTest, Constructor_Double_SetsValues) {
    Token token(TokenType::NUMBER, 3.14, 3, 15);
    
    EXPECT_EQ(token.type, TokenType::NUMBER);
    EXPECT_DOUBLE_EQ(token.asDouble(), 3.14);
}

TEST_F(TokenTest, AsInt_ConvertsFromDouble) {
    Token token(TokenType::NUMBER, 42.7, 1, 1);
    
    EXPECT_EQ(token.asInt(), 42);
}

TEST_F(TokenTest, AsDouble_ConvertsFromInt) {
    Token token(TokenType::NUMBER, 42, 1, 1);
    
    EXPECT_DOUBLE_EQ(token.asDouble(), 42.0);
}

TEST_F(TokenTest, AsString_ReturnsStoredString) {
    Token token(TokenType::IDENTIFIER, std::string("hello"), 1, 1);
    
    EXPECT_EQ(token.asString(), "hello");
}

TEST_F(TokenTest, ToString_IncludesTypeAndValue) {
    Token token(TokenType::COMMAND, std::string("create"), 1, 5);
    std::string str = token.toString();
    
    EXPECT_NE(str.find("create"), std::string::npos);
    EXPECT_NE(str.find("1:5"), std::string::npos);
}