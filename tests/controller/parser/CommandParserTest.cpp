#include <gtest/gtest.h>
#include "controller/parser/CommandParser.hpp"
#include "controller/CommandRegistry.hpp"
#include "controller/MetaCommand.hpp"
#include "io/InputStream.hpp"
#include <memory>
#include <sstream>

using namespace slideEditor::controller;
using namespace slideEditor;

class CommandParserTest : public ::testing::Test {
protected:
    std::unique_ptr<CommandParser> parser_;
    std::unique_ptr<CommandRegistry> registry_;
    std::unique_ptr<std::stringstream> inputStream_;
    std::unique_ptr<io::InputStream> input_;

    void SetUp() override {
        registry_ = std::make_unique<CommandRegistry>();
        setupTestCommands();
    }

    void setupTestCommands() {
        // Register a simple command with no arguments
        auto simpleCmd = std::make_unique<MetaCommand>(
            "simple",
            "A simple command",
            [](const std::vector<std::string>&) { return true; }
        );
        registry_->registerCommand(std::move(simpleCmd));

        // Register a command with required int argument
        auto intCmd = std::make_unique<MetaCommand>(
            "setid",
            "Set ID command",
            [](const std::vector<std::string>&) { return true; }
        );
        intCmd->addArgument("id", "int", true);
        registry_->registerMetaCommand(std::move(intCmd));

        // Register a command with double argument
        auto doubleCmd = std::make_unique<MetaCommand>(
            "setsize",
            "Set size command",
            [](const std::vector<std::string>&) { return true; }
        );
        doubleCmd->addArgument("size", "double", true);
        registry_->registerMetaCommand(std::move(doubleCmd));

        // Register a command with string argument
        auto stringCmd = std::make_unique<MetaCommand>(
            "setname",
            "Set name command",
            [](const std::vector<std::string>&) { return true; }
        );
        stringCmd->addArgument("name", "string", true);
        registry_->registerMetaCommand(std::move(stringCmd));

        // Register a command with multiple arguments
        auto multiCmd = std::make_unique<MetaCommand>(
            "create",
            "Create command",
            [](const std::vector<std::string>&) { return true; }
        );
        multiCmd->addArgument("id", "int", true);
        multiCmd->addArgument("name", "string", true);
        multiCmd->addArgument("size", "double", true);
        registry_->registerMetaCommand(std::move(multiCmd));

        // Register a command with optional argument
        auto optionalCmd = std::make_unique<MetaCommand>(
            "optional",
            "Optional argument command",
            [](const std::vector<std::string>&) { return true; }
        );
        optionalCmd->addArgument("required", "int", true);
        optionalCmd->addArgument("optional", "string", false);
        registry_->registerMetaCommand(std::move(optionalCmd));
    }

    void createParser(const std::string& input) {
        inputStream_ = std::make_unique<std::stringstream>(input);
        input_ = std::make_unique<io::InputStream>(*inputStream_);
        parser_ = std::make_unique<CommandParser>(input_.get());
        parser_->setRegistry(registry_.get());
    }
};

TEST_F(CommandParserTest, Constructor_InitializesCorrectly) {
    createParser("simple\n");
    EXPECT_NO_THROW(parser_->parseCommand());
}

TEST_F(CommandParserTest, ParseCommand_SimpleCommand) {
    createParser("simple\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_TRUE(cmd.isValid);
    EXPECT_EQ(cmd.commandName, "simple");
    EXPECT_EQ(cmd.arguments.size(), 0);
}

TEST_F(CommandParserTest, ParseCommand_CommandWithIntArgument) {
    createParser("setid 42\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_TRUE(cmd.isValid);
    EXPECT_EQ(cmd.commandName, "setid");
    EXPECT_EQ(cmd.arguments.size(), 1);
    EXPECT_EQ(cmd.arguments[0].type, "int");
    EXPECT_EQ(cmd.arguments[0].asInt(), 42);
}

TEST_F(CommandParserTest, ParseCommand_CommandWithDoubleArgument) {
    createParser("setsize 3.14\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_TRUE(cmd.isValid);
    EXPECT_EQ(cmd.commandName, "setsize");
    EXPECT_EQ(cmd.arguments.size(), 1);
    EXPECT_EQ(cmd.arguments[0].type, "double");
    EXPECT_DOUBLE_EQ(cmd.arguments[0].asDouble(), 3.14);
}

TEST_F(CommandParserTest, ParseCommand_CommandWithStringArgument) {
    createParser("setname TestName\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_TRUE(cmd.isValid);
    EXPECT_EQ(cmd.commandName, "setname");
    EXPECT_EQ(cmd.arguments.size(), 1);
    EXPECT_EQ(cmd.arguments[0].type, "string");
    EXPECT_EQ(cmd.arguments[0].asString(), "TestName");
}

TEST_F(CommandParserTest, ParseCommand_CommandWithMultipleArguments) {
    createParser("create 1 Item 2.5\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_TRUE(cmd.isValid);
    EXPECT_EQ(cmd.commandName, "create");
    EXPECT_EQ(cmd.arguments.size(), 3);
    EXPECT_EQ(cmd.arguments[0].asInt(), 1);
    EXPECT_EQ(cmd.arguments[1].asString(), "Item");
    EXPECT_DOUBLE_EQ(cmd.arguments[2].asDouble(), 2.5);
}

TEST_F(CommandParserTest, ParseCommand_CaseInsensitiveCommand) {
    createParser("SIMPLE\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_TRUE(cmd.isValid);
    EXPECT_EQ(cmd.commandName, "simple");
}

TEST_F(CommandParserTest, ParseCommand_UnknownCommand) {
    createParser("unknown\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_FALSE(cmd.isValid);
    EXPECT_NE(cmd.errorMessage.find("Unknown command"), std::string::npos);
}

TEST_F(CommandParserTest, ParseCommand_MissingRequiredArgument) {
    createParser("setid\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_FALSE(cmd.isValid);
    EXPECT_NE(cmd.errorMessage.find("Missing required argument"), std::string::npos);
}

TEST_F(CommandParserTest, ParseCommand_InvalidIntArgument) {
    createParser("setid notanumber\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_FALSE(cmd.isValid);
}

TEST_F(CommandParserTest, ParseCommand_OptionalArgumentPresent) {
    createParser("optional 10 value\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_TRUE(cmd.isValid);
    EXPECT_EQ(cmd.arguments.size(), 2);
    EXPECT_EQ(cmd.arguments[0].asInt(), 10);
    EXPECT_EQ(cmd.arguments[1].asString(), "value");
}

TEST_F(CommandParserTest, ParseCommand_OptionalArgumentMissing) {
    createParser("optional 10\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_TRUE(cmd.isValid);
    EXPECT_EQ(cmd.arguments.size(), 1);
    EXPECT_EQ(cmd.arguments[0].asInt(), 10);
}

TEST_F(CommandParserTest, ParseCommand_EndOfFile) {
    createParser("");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_FALSE(cmd.isValid);
    EXPECT_EQ(cmd.errorMessage, "End of input");
}

TEST_F(CommandParserTest, ParseCommand_NoCommandKeyword) {
    createParser("123\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_FALSE(cmd.isValid);
    EXPECT_NE(cmd.errorMessage.find("Expected command keyword"), std::string::npos);
}

TEST_F(CommandParserTest, ParseCommand_NoRegistrySet) {
    createParser("simple\n");
    parser_ = std::make_unique<CommandParser>(input_.get());
    // Don't set registry
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_FALSE(cmd.isValid);
    EXPECT_NE(cmd.errorMessage.find("Command registry not set"), std::string::npos);
}

TEST_F(CommandParserTest, ParseAll_MultipleCommands) {
    createParser("simple\nsetid 10\nsetname Test\n");
    std::vector<ParsedCommand> commands = parser_->parseAll();
    
    EXPECT_EQ(commands.size(), 3);
    EXPECT_TRUE(commands[0].isValid);
    EXPECT_EQ(commands[0].commandName, "simple");
    EXPECT_TRUE(commands[1].isValid);
    EXPECT_EQ(commands[1].commandName, "setid");
    EXPECT_TRUE(commands[2].isValid);
    EXPECT_EQ(commands[2].commandName, "setname");
}

TEST_F(CommandParserTest, ParseAll_EmptyInput) {
    createParser("");
    std::vector<ParsedCommand> commands = parser_->parseAll();
    
    EXPECT_EQ(commands.size(), 1);
    EXPECT_FALSE(commands[0].isValid);
}

TEST_F(CommandParserTest, ParseAll_MixedValidInvalid) {
    createParser("simple\nunknown\nsetid 42\n");
    std::vector<ParsedCommand> commands = parser_->parseAll();
    
    EXPECT_EQ(commands.size(), 3);
    EXPECT_TRUE(commands[0].isValid);
    EXPECT_FALSE(commands[1].isValid);
    EXPECT_TRUE(commands[2].isValid);
}

TEST_F(CommandParserTest, ParsedArgument_IntConversions) {
    ParsedArgument arg(42);
    EXPECT_EQ(arg.asInt(), 42);
    EXPECT_DOUBLE_EQ(arg.asDouble(), 42.0);
    EXPECT_EQ(arg.asString(), "42");
}

TEST_F(CommandParserTest, ParsedArgument_DoubleConversions) {
    ParsedArgument arg(3.14);
    EXPECT_EQ(arg.asInt(), 3);
    EXPECT_DOUBLE_EQ(arg.asDouble(), 3.14);
    EXPECT_EQ(arg.asString(), "3.140000");
}

TEST_F(CommandParserTest, ParsedArgument_StringConversions) {
    ParsedArgument arg(std::string("test"));
    EXPECT_EQ(arg.asString(), "test");
}

TEST_F(CommandParserTest, ParsedCommand_GetArgumentStrings) {
    createParser("create 1 Item 2.5\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    std::vector<std::string> argStrings = cmd.getArgumentStrings();
    EXPECT_EQ(argStrings.size(), 3);
    EXPECT_EQ(argStrings[0], "1");
    EXPECT_EQ(argStrings[1], "Item");
}

TEST_F(CommandParserTest, ParseCommand_SkipsEndOfLineTokens) {
    createParser("simple\n\n\nsetid 10\n");
    ParsedCommand cmd1 = parser_->parseCommand();
    ParsedCommand cmd2 = parser_->parseCommand();
    
    EXPECT_TRUE(cmd1.isValid);
    EXPECT_EQ(cmd1.commandName, "simple");
    EXPECT_TRUE(cmd2.isValid);
    EXPECT_EQ(cmd2.commandName, "setid");
}

TEST_F(CommandParserTest, ParseCommand_NegativeNumbers) {
    createParser("setid -42\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_TRUE(cmd.isValid);
    EXPECT_EQ(cmd.arguments[0].asInt(), -42);
}

TEST_F(CommandParserTest, ParseCommand_NegativeDouble) {
    createParser("setsize -3.14\n");
    ParsedCommand cmd = parser_->parseCommand();
    
    EXPECT_TRUE(cmd.isValid);
    EXPECT_DOUBLE_EQ(cmd.arguments[0].asDouble(), -3.14);
}