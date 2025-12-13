#include <gtest/gtest.h>
#include "io/InputStream.hpp"
#include <sstream>

using namespace slideEditor::io;

class InputStreamTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(InputStreamTest, Constructor_WithString_StoresString) {
    InputStream stream("test input");
    
    EXPECT_FALSE(stream.eof());
    EXPECT_TRUE(stream.good());
}

TEST_F(InputStreamTest, Constructor_WithIstream_UsesStream) {
    std::istringstream iss("test");
    InputStream stream(iss);
    
    EXPECT_FALSE(stream.eof());
    EXPECT_TRUE(stream.good());
}

TEST_F(InputStreamTest, Get_ReturnsCharacters) {
    InputStream stream("abc");
    
    auto ch1 = stream.get();
    auto ch2 = stream.get();
    auto ch3 = stream.get();
    
    ASSERT_TRUE(ch1.has_value());
    ASSERT_TRUE(ch2.has_value());
    ASSERT_TRUE(ch3.has_value());
    
    EXPECT_EQ(ch1.value(), 'a');
    EXPECT_EQ(ch2.value(), 'b');
    EXPECT_EQ(ch3.value(), 'c');
}

TEST_F(InputStreamTest, Get_ReturnsNulloptWhenEmpty) {
    InputStream stream("");
    
    auto ch = stream.get();
    
    EXPECT_FALSE(ch.has_value());
}

TEST_F(InputStreamTest, Peek_DoesNotAdvance) {
    InputStream stream("ab");
    
    auto peek1 = stream.peek();
    auto peek2 = stream.peek();
    auto get1 = stream.get();
    
    ASSERT_TRUE(peek1.has_value());
    ASSERT_TRUE(peek2.has_value());
    ASSERT_TRUE(get1.has_value());
    
    EXPECT_EQ(peek1.value(), 'a');
    EXPECT_EQ(peek2.value(), 'a');
    EXPECT_EQ(get1.value(), 'a');
}

TEST_F(InputStreamTest, Peek_ReturnsNulloptAtEnd) {
    InputStream stream("a");
    
    stream.get();
    auto peek = stream.peek();
    
    EXPECT_FALSE(peek.has_value());
}

TEST_F(InputStreamTest, Eof_ReturnsTrueWhenExhausted) {
    InputStream stream("a");
    
    EXPECT_FALSE(stream.eof());
    stream.get();
    stream.get(); // Try to read past end
    EXPECT_TRUE(stream.eof());
}

TEST_F(InputStreamTest, Good_ReturnsFalseAfterEof) {
    InputStream stream("a");
    
    EXPECT_TRUE(stream.good());
    stream.get();
    stream.get(); // Past end
    EXPECT_FALSE(stream.good());
}

TEST_F(InputStreamTest, Unget_PutsBackCharacter) {
    InputStream stream("abc");
    
    auto ch1 = stream.get();
    stream.unget();
    auto ch2 = stream.get();
    
    ASSERT_TRUE(ch1.has_value());
    ASSERT_TRUE(ch2.has_value());
    EXPECT_EQ(ch1.value(), ch2.value());
    EXPECT_EQ(ch2.value(), 'a');
}

TEST_F(InputStreamTest, Unget_WorksOnce) {
    InputStream stream("abc");
    
    stream.get(); // 'a'
    stream.unget();
    stream.unget(); // Second unget might not work
    
    auto ch = stream.get();
    ASSERT_TRUE(ch.has_value());
    EXPECT_EQ(ch.value(), 'a');
}

TEST_F(InputStreamTest, MultipleGetsAndPeeks) {
    InputStream stream("test");
    
    auto p1 = stream.peek();
    auto g1 = stream.get();
    auto g2 = stream.get();
    auto p2 = stream.peek();
    auto g3 = stream.get();
    
    EXPECT_EQ(p1.value(), 't');
    EXPECT_EQ(g1.value(), 't');
    EXPECT_EQ(g2.value(), 'e');
    EXPECT_EQ(p2.value(), 's');
    EXPECT_EQ(g3.value(), 's');
}

TEST_F(InputStreamTest, EmptyString_EofImmediately) {
    InputStream stream("");
    
    EXPECT_TRUE(stream.eof());
    EXPECT_FALSE(stream.good());
}

TEST_F(InputStreamTest, ReadEntireString) {
    InputStream stream("hello");
    std::string result;
    
    while (auto ch = stream.get()) {
        result += ch.value();
    }
    
    EXPECT_EQ(result, "hello");
    EXPECT_TRUE(stream.eof());
}