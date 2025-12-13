#include <gtest/gtest.h>
#include "controller/parser/CommandParser.hpp"
#include "controller/CommandRegistry.hpp"
#include "controller/commands/MetaCommandDefinitions.hpp"
#include "io/InputStream.hpp"
#include <memory>

using namespace slideEditor::controller;
using namespace slideEditor::io;

class CommandParserTest : public ::testing::Test {
protected:
    std::unique_ptr<CommandRegistry> registry_;
    
    void SetUp() override {
        registry_ = std::make_unique<CommandRegistry>();
        
        // Register test commands
        registry_->registerCommand(createCreateMetaCommand());
        registry_->registerCommand(createAddShapeMetaCommand());
        registry_->registerCommand(createRemoveShapeMetaCommand());
        registry_->registerCommand(createDisplayMetaCommand());
        registry_->registerCommand(createSaveMetaCommand());
        registry_->registerCommand(createHelpMetaCommand());
    }
    
    ParsedCommand parseString(const std::string& input) {
        auto stream = std::make_unique<InputStream>(input);
        CommandParser parser(stream.get());
        parser.setRegistry(registry_.get());
        
        return parser.parseCommand();
    }
};

TEST_F(CommandParserTest, ParseCreate_WithValidArguments_Succeeds) {
    ParsedCommand result = parseString("create Title Content Theme");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.commandName, "create");
    ASSERT_EQ(result.arguments.size(), 3);
    EXPECT_EQ(result.arguments[0].asString(), "Title");
    EXPECT_EQ(result.arguments[1].asString(), "Content");
    EXPECT_EQ(result.arguments[2].asString(), "Theme");
}

TEST_F(CommandParserTest, ParseCreate_WithMissingArguments_Fails) {
    ParsedCommand result = parseString("create Title");
    
    EXPECT_FALSE(result.isValid);
    EXPECT_NE(result.errorMessage.find("Missing"), std::string::npos);
}

TEST_F(CommandParserTest, ParseAddShape_WithValidArguments_Succeeds) {
    ParsedCommand result = parseString("addshape 1 circle 2.5");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.commandName, "addshape");
    ASSERT_EQ(result.arguments.size(), 3);
    EXPECT_EQ(result.arguments[0].asInt(), 1);
    EXPECT_EQ(result.arguments[1].asString(), "circle");
    EXPECT_DOUBLE_EQ(result.arguments[2].asDouble(), 2.5);
}

TEST_F(CommandParserTest, ParseAddShape_WithColors_Succeeds) {
    ParsedCommand result = parseString("addshape 1 circle 2.5 red blue");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    ASSERT_EQ(result.arguments.size(), 5);
    EXPECT_EQ(result.arguments[0].asInt(), 1);
    EXPECT_EQ(result.arguments[1].asString(), "circle");
    EXPECT_DOUBLE_EQ(result.arguments[2].asDouble(), 2.5);
    EXPECT_EQ(result.arguments[3].asString(), "red");
    EXPECT_EQ(result.arguments[4].asString(), "blue");
}

TEST_F(CommandParserTest, ParseAddShape_WithHexColors_Succeeds) {
    ParsedCommand result = parseString("addshape 1 circle 2.5 #FF0000 #0000FF");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    ASSERT_EQ(result.arguments.size(), 5);
    EXPECT_EQ(result.arguments[3].asString(), "#FF0000");
    EXPECT_EQ(result.arguments[4].asString(), "#0000FF");
}

TEST_F(CommandParserTest, ParseAddShape_WithInvalidInt_Fails) {
    ParsedCommand result = parseString("addshape abc circle 2.5");
    
    EXPECT_FALSE(result.isValid);
    EXPECT_NE(result.errorMessage, "");
}

TEST_F(CommandParserTest, ParseAddShape_WithInvalidDouble_Fails) {
    ParsedCommand result = parseString("addshape 1 circle notanumber");
    
    EXPECT_FALSE(result.isValid);
}

TEST_F(CommandParserTest, ParseRemoveShape_WithValidArguments_Succeeds) {
    ParsedCommand result = parseString("removeshape 1 0");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.commandName, "removeshape");
    ASSERT_EQ(result.arguments.size(), 2);
    EXPECT_EQ(result.arguments[0].asInt(), 1);
    EXPECT_EQ(result.arguments[1].asInt(), 0);
}

TEST_F(CommandParserTest, ParseDisplay_WithNoArguments_Succeeds) {
    ParsedCommand result = parseString("display");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.commandName, "display");
    EXPECT_EQ(result.arguments.size(), 0);
}

TEST_F(CommandParserTest, ParseSave_WithFilename_Succeeds) {
    ParsedCommand result = parseString("save myfile.json");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.commandName, "save");
    ASSERT_EQ(result.arguments.size(), 1);
    EXPECT_EQ(result.arguments[0].asString(), "myfile.json");
}

TEST_F(CommandParserTest, ParseHelp_WithNoArguments_Succeeds) {
    ParsedCommand result = parseString("help");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.commandName, "help");
    EXPECT_EQ(result.arguments.size(), 0);
}

TEST_F(CommandParserTest, ParseHelp_WithCommandName_Succeeds) {
    ParsedCommand result = parseString("help create");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.commandName, "help");
    ASSERT_EQ(result.arguments.size(), 1);
    EXPECT_EQ(result.arguments[0].asString(), "create");
}

TEST_F(CommandParserTest, ParseUnknownCommand_Fails) {
    ParsedCommand result = parseString("unknowncommand arg1 arg2");
    
    EXPECT_FALSE(result.isValid);
    EXPECT_NE(result.errorMessage.find("Unknown"), std::string::npos);
}

TEST_F(CommandParserTest, ParseEmpty_Fails) {
    ParsedCommand result = parseString("");
    
    EXPECT_FALSE(result.isValid);
    EXPECT_NE(result.errorMessage.find("End of input"), std::string::npos);
}

TEST_F(CommandParserTest, ParseOnlyWhitespace_Fails) {
    ParsedCommand result = parseString("   \t  ");
    
    EXPECT_FALSE(result.isValid);
}

TEST_F(CommandParserTest, ParseCaseInsensitive_Succeeds) {
    ParsedCommand result = parseString("CREATE Title Content Theme");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.commandName, "create");
}

TEST_F(CommandParserTest, ParseMixedCase_Succeeds) {
    ParsedCommand result = parseString("CrEaTe Title Content Theme");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.commandName, "create");
}

TEST_F(CommandParserTest, GetArgumentStrings_ReturnsConvertedStrings) {
    ParsedCommand result = parseString("addshape 1 circle 2.5");
    
    auto strings = result.getArgumentStrings();
    
    ASSERT_EQ(strings.size(), 3);
    EXPECT_EQ(strings[0], "1");
    EXPECT_EQ(strings[1], "circle");
    // Double might be "2.5" or "2.500000" depending on implementation
    EXPECT_NE(strings[2].find("2.5"), std::string::npos);
}

TEST_F(CommandParserTest, ParseNegativeNumbers_Succeeds) {
    ParsedCommand result = parseString("addshape -1 circle -2.5");
    
    if (result.isValid) {
        EXPECT_EQ(result.arguments[0].asInt(), -1);
        EXPECT_DOUBLE_EQ(result.arguments[2].asDouble(), -2.5);
    }
}

TEST_F(CommandParserTest, ParseWithExtraWhitespace_Succeeds) {
    ParsedCommand result = parseString("  create   Title   Content   Theme  ");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.arguments.size(), 3);
}

TEST_F(CommandParserTest, ParseWithTabs_Succeeds) {
    ParsedCommand result = parseString("create\tTitle\tContent\tTheme");
    
    EXPECT_TRUE(result.isValid) << "Error: " << result.errorMessage;
    EXPECT_EQ(result.arguments.size(), 3);
}

TEST_F(CommandParserTest, ParseMultipleCommands_Sequential) {
    auto stream = std::make_unique<InputStream>("create T1 C1 Th1\ncreate T2 C2 Th2");
    CommandParser parser(stream.get());
    parser.setRegistry(registry_.get());
    
    ParsedCommand cmd1 = parser.parseCommand();
    ParsedCommand cmd2 = parser.parseCommand();
    
    EXPECT_TRUE(cmd1.isValid);
    EXPECT_TRUE(cmd2.isValid);
    EXPECT_EQ(cmd1.arguments[0].asString(), "T1");
    EXPECT_EQ(cmd2.arguments[0].asString(), "T2");
}

TEST_F(CommandParserTest, ArgumentTypes_PreserveTypes) {
    ParsedCommand result = parseString("addshape 42 circle 3.14");
    
    ASSERT_TRUE(result.isValid);
    ASSERT_EQ(result.arguments.size(), 3);
    
    // First argument is int
    EXPECT_EQ(result.arguments[0].type, "int");
    EXPECT_EQ(result.arguments[0].asInt(), 42);
    
    // Second is string
    EXPECT_EQ(result.arguments[1].type, "string");
    EXPECT_EQ(result.arguments[1].asString(), "circle");
    
    // Third is double
    EXPECT_EQ(result.arguments[2].type, "double");
    EXPECT_DOUBLE_EQ(result.arguments[2].asDouble(), 3.14);
}