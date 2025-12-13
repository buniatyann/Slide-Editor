#include <gtest/gtest.h>
#include "io/OutputStream.hpp"
#include <sstream>

using namespace slideEditor::io;

class OutputStreamTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(OutputStreamTest, Constructor_WithOstream_UsesProvidedStream) {
    std::ostringstream buffer;
    OutputStream stream(buffer);
    
    stream.write("test");
    EXPECT_EQ(buffer.str(), "test");
}

TEST_F(OutputStreamTest, DefaultConstructor_CreatesOwnStream) {
    OutputStream stream;
    
    stream.write("test");
    EXPECT_EQ(stream.getOutput(), "test");
}

TEST_F(OutputStreamTest, Write_WritesToStream) {
    std::ostringstream buffer;
    OutputStream stream(buffer);
    
    stream.write("hello");
    EXPECT_EQ(buffer.str(), "hello");
}

TEST_F(OutputStreamTest, WriteLine_AddsNewline) {
    std::ostringstream buffer;
    OutputStream stream(buffer);
    
    stream.writeLine("test");
    EXPECT_EQ(buffer.str(), "test\n");
}

TEST_F(OutputStreamTest, MultipleWrites_Concatenate) {
    std::ostringstream buffer;
    OutputStream stream(buffer);
    
    stream.write("hello");
    stream.write(" ");
    stream.write("world");
    
    EXPECT_EQ(buffer.str(), "hello world");
}

TEST_F(OutputStreamTest, Flush_Works) {
    std::ostringstream buffer;
    OutputStream stream(buffer);
    
    stream.write("test");
    stream.flush();
    
    EXPECT_EQ(buffer.str(), "test");
}

TEST_F(OutputStreamTest, Good_ReturnsStreamState) {
    std::ostringstream buffer;
    OutputStream stream(buffer);
    
    EXPECT_TRUE(stream.good());
}

TEST_F(OutputStreamTest, Fail_ReturnsStreamState) {
    std::ostringstream buffer;
    OutputStream stream(buffer);
    
    EXPECT_FALSE(stream.fail());
}

TEST_F(OutputStreamTest, GetOutput_ReturnsStringFromOwnedStream) {
    OutputStream stream;  // Uses default constructor
    
    stream.write("line1");
    stream.writeLine("");
    stream.write("line2");
    
    std::string output = stream.getOutput();
    EXPECT_NE(output.find("line1"), std::string::npos);
    EXPECT_NE(output.find("line2"), std::string::npos);
}

TEST_F(OutputStreamTest, GetOutput_ReturnsEmptyForExternalStream) {
    std::ostringstream buffer;
    OutputStream stream(buffer);
    
    stream.write("test");
    
    EXPECT_EQ(stream.getOutput(), "");  //  correct behavior
    
    // But the external buffer should have the data
    EXPECT_EQ(buffer.str(), "test");
}