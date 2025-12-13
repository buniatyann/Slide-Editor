#include <gtest/gtest.h>
#include "io/OutputStream.hpp"
#include <sstream>

using namespace slideEditor::io;

class OutputStreamTest : public ::testing::Test {
protected:
    std::ostringstream buffer_;
    
    void SetUp() override {
        buffer_.str("");
        buffer_.clear();
    }
};

TEST_F(OutputStreamTest, Constructor_WithOstream_CreatesStream) {
    OutputStream stream(buffer_);
    
    // Stream should be ready to use
    stream.write("test");
    EXPECT_EQ(buffer_.str(), "test");
}

TEST_F(OutputStreamTest, Write_String_WritesToBuffer) {
    OutputStream stream(buffer_);
    
    stream.write("hello");
    
    EXPECT_EQ(buffer_.str(), "hello");
}

TEST_F(OutputStreamTest, Write_CString_WritesToBuffer) {
    OutputStream stream(buffer_);
    
    stream.write("test");
    
    EXPECT_EQ(buffer_.str(), "test");
}

TEST_F(OutputStreamTest, WriteLine_AddsNewline) {
    OutputStream stream(buffer_);
    
    stream.writeLine("test");
    
    EXPECT_EQ(buffer_.str(), "test\n");
}

TEST_F(OutputStreamTest, WriteLine_EmptyString_WritesOnlyNewline) {
    OutputStream stream(buffer_);
    
    stream.writeLine("");
    
    EXPECT_EQ(buffer_.str(), "\n");
}

TEST_F(OutputStreamTest, MultipleWrites_Concatenate) {
    OutputStream stream(buffer_);
    
    stream.write("hello");
    stream.write(" ");
    stream.write("world");
    
    EXPECT_EQ(buffer_.str(), "hello world");
}

TEST_F(OutputStreamTest, MultipleWriteLines_EachHasNewline) {
    OutputStream stream(buffer_);
    
    stream.writeLine("line1");
    stream.writeLine("line2");
    stream.writeLine("line3");
    
    EXPECT_EQ(buffer_.str(), "line1\nline2\nline3\n");
}

TEST_F(OutputStreamTest, MixedWriteAndWriteLine) {
    OutputStream stream(buffer_);
    
    stream.write("prefix: ");
    stream.writeLine("content");
    stream.write("suffix");
    
    EXPECT_EQ(buffer_.str(), "prefix: content\nsuffix");
}

TEST_F(OutputStreamTest, WriteInt_ConvertsToString) {
    OutputStream stream(buffer_);
    
    stream.write(std::to_string(42));
    
    EXPECT_EQ(buffer_.str(), "42");
}

TEST_F(OutputStreamTest, WriteNegativeInt_ConvertsToString) {
    OutputStream stream(buffer_);
    
    stream.write(std::to_string(-123));
    
    EXPECT_EQ(buffer_.str(), "-123");
}

TEST_F(OutputStreamTest, WriteDouble_ConvertsToString) {
    OutputStream stream(buffer_);
    
    stream.write(std::to_string(3.14));
    
    std::string result = buffer_.str();
    EXPECT_NE(result.find("3.14"), std::string::npos);
}

TEST_F(OutputStreamTest, WriteBool_True_WritesTrue) {
    OutputStream stream(buffer_);
    
    stream.write("true");
    
    EXPECT_EQ(buffer_.str(), "true");
}

TEST_F(OutputStreamTest, WriteBool_False_WritesFalse) {
    OutputStream stream(buffer_);
    
    stream.write("false");
    
    EXPECT_EQ(buffer_.str(), "false");
}

TEST_F(OutputStreamTest, WriteMixedTypes) {
    OutputStream stream(buffer_);
    
    stream.write("Value: ");
    stream.write(std::to_string(42));
    stream.write(", ");
    stream.write("true");
    stream.write(", ");
    stream.write(std::to_string(3.14));
    
    std::string result = buffer_.str();
    EXPECT_NE(result.find("Value: 42"), std::string::npos);
    EXPECT_NE(result.find("true"), std::string::npos);
    EXPECT_NE(result.find("3.14"), std::string::npos);
}

TEST_F(OutputStreamTest, WriteChar_Single) {
    OutputStream stream(buffer_);
    
    stream.write("A");
    
    EXPECT_EQ(buffer_.str(), "A");
}

TEST_F(OutputStreamTest, LargeOutput_HandlesCorrectly) {
    OutputStream stream(buffer_);
    
    for (int i = 0; i < 1000; ++i) {
        stream.write(std::to_string(i));
        if (i < 999) 
            stream.write(" ");
    }
    
    std::string result = buffer_.str();
    EXPECT_NE(result.find("0"), std::string::npos);
    EXPECT_NE(result.find("999"), std::string::npos);
}

TEST_F(OutputStreamTest, Flush_EnsuresDataWritten) {
    OutputStream stream(buffer_);
    
    stream.write("test");
    stream.flush();
    
    EXPECT_EQ(buffer_.str(), "test");
}