#include <gtest/gtest.h>
#include "controller/CommandRegistry.hpp"
#include "controller/MetaCommand.hpp"

using namespace slideEditor::controller;
using namespace slideEditor::core;

class CommandRegistryTest : public ::testing::Test {
protected:
    CommandRegistry registry_;
    
    void SetUp() override {}
    
    std::unique_ptr<IMetaCommand> createTestMetaCommand(const std::string& name) {
        auto creator = [](const std::vector<std::string>&, void*) -> std::unique_ptr<ICommand> {
            return nullptr;
        };
        
        return std::make_unique<MetaCommand>(
            name,
            "Test command",
            "TEST",
            creator
        );
    }
};

TEST_F(CommandRegistryTest, RegisterCommand_AddsCommand) {
    registry_.registerCommand(createTestMetaCommand("testcmd"));
    
    EXPECT_TRUE(registry_.hasCommand("testcmd"));
}

TEST_F(CommandRegistryTest, RegisterCommand_CaseInsensitive) {
    registry_.registerCommand(createTestMetaCommand("TestCmd"));
    
    EXPECT_TRUE(registry_.hasCommand("testcmd"));
    EXPECT_TRUE(registry_.hasCommand("TESTCMD"));
    EXPECT_TRUE(registry_.hasCommand("TestCmd"));
}

TEST_F(CommandRegistryTest, GetMetaCommand_ReturnsCommand) {
    registry_.registerCommand(createTestMetaCommand("testcmd"));
    
    const auto* metaCmd = registry_.getMetaCommand("testcmd");
    
    ASSERT_NE(metaCmd, nullptr);
    EXPECT_EQ(metaCmd->getName(), "testcmd");
}

TEST_F(CommandRegistryTest, GetMetaCommand_ReturnsNullForUnknown) {
    const auto* metaCmd = registry_.getMetaCommand("unknown");
    
    EXPECT_EQ(metaCmd, nullptr);
}

TEST_F(CommandRegistryTest, HasCommand_ReturnsFalseForUnknown) {
    EXPECT_FALSE(registry_.hasCommand("unknown"));
}

TEST_F(CommandRegistryTest, GetAllCommandNames_ReturnsNames) {
    registry_.registerCommand(createTestMetaCommand("cmd1"));
    registry_.registerCommand(createTestMetaCommand("cmd2"));
    registry_.registerCommand(createTestMetaCommand("cmd3"));
    
    auto names = registry_.getAllCommandNames();
    
    EXPECT_EQ(names.size(), 3);
}

TEST_F(CommandRegistryTest, GetCommandsByCategory_FiltersCorrectly) {
    auto creator = [](const std::vector<std::string>&, void*) -> std::unique_ptr<ICommand> {
        return nullptr;
    };
    
    registry_.registerCommand(std::make_unique<MetaCommand>(
        "action1", "Test", "ACTION", creator
    ));
    registry_.registerCommand(std::make_unique<MetaCommand>(
        "query1", "Test", "QUERY", creator
    ));
    registry_.registerCommand(std::make_unique<MetaCommand>(
        "action2", "Test", "ACTION", creator
    ));
    
    auto actions = registry_.getCommandsByCategory("ACTION");
    
    EXPECT_EQ(actions.size(), 2);
}

TEST_F(CommandRegistryTest, GetCommandHelp_ReturnsHelp) {
    registry_.registerCommand(createTestMetaCommand("testcmd"));
    
    std::string help = registry_.getCommandHelp("testcmd");
    
    EXPECT_FALSE(help.empty());
    EXPECT_NE(help.find("testcmd"), std::string::npos);
}

TEST_F(CommandRegistryTest, GetAllCommandsHelp_ReturnsFormattedHelp) {
    registry_.registerCommand(createTestMetaCommand("cmd1"));
    registry_.registerCommand(createTestMetaCommand("cmd2"));
    
    std::string help = registry_.getAllCommandsHelp();
    
    EXPECT_NE(help.find("cmd1"), std::string::npos);
    EXPECT_NE(help.find("cmd2"), std::string::npos);
}