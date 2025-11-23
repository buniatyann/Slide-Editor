#include <gtest/gtest.h>
#include "controller/CommandContext.hpp"
#include "model/SlideRepository.hpp"
#include "serialization/JsonSerializer.hpp"
#include "view/cli/CliView.hpp"
#include "controller/CommandHistory.hpp"
#include "controller/CommandRegistry.hpp"
#include <memory>
#include <sstream>

using namespace slideEditor::controller;
using namespace slideEditor;

class CommandContextTest : public ::testing::Test {
protected:
    std::shared_ptr<model::SlideRepository> repository_;
    std::shared_ptr<serialization::JsonSerializer> serializer_;
    std::shared_ptr<view::CliView> view_;
    std::shared_ptr<CommandHistory> history_;
    std::unique_ptr<CommandRegistry> registry_;
    std::ostringstream output_;
    
    void SetUp() override {
        repository_ = std::make_shared<model::SlideRepository>();
        serializer_ = std::make_shared<serialization::JsonSerializer>();
        view_ = std::make_shared<view::CliView>(output_);
        history_ = std::make_shared<CommandHistory>(100);
        registry_ = std::make_unique<CommandRegistry>();
    }
};

TEST_F(CommandContextTest, DefaultConstructor_CreatesEmptyContext) {
    CommandContext context;
    
    EXPECT_FALSE(context.hasRepository());
    EXPECT_FALSE(context.hasSerializer());
    EXPECT_FALSE(context.hasView());
    EXPECT_FALSE(context.hasHistory());
    EXPECT_FALSE(context.hasRegistry());
    EXPECT_FALSE(context.isValid());
}

TEST_F(CommandContextTest, SetRepository_StoresRepository) {
    CommandContext context;
    context.setRepository(repository_);
    
    EXPECT_TRUE(context.hasRepository());
    EXPECT_EQ(context.getRepository(), repository_);
}

TEST_F(CommandContextTest, SetSerializer_StoresSerializer) {
    CommandContext context;
    context.setSerializer(serializer_);
    
    EXPECT_TRUE(context.hasSerializer());
    EXPECT_EQ(context.getSerializer(), serializer_);
}

TEST_F(CommandContextTest, SetView_StoresView) {
    CommandContext context;
    context.setView(view_);
    
    EXPECT_TRUE(context.hasView());
    EXPECT_EQ(context.getView(), view_);
}

TEST_F(CommandContextTest, SetHistory_StoresHistory) {
    CommandContext context;
    context.setHistory(history_);
    
    EXPECT_TRUE(context.hasHistory());
    EXPECT_NE(context.getHistory(), nullptr);
}

TEST_F(CommandContextTest, SetRegistry_StoresRegistry) {
    CommandContext context;
    context.setRegistry(registry_.get());
    
    EXPECT_TRUE(context.hasRegistry());
    EXPECT_EQ(context.getRegistry(), registry_.get());
}

TEST_F(CommandContextTest, IsValid_ReturnsFalse_WhenMissingDependencies) {
    CommandContext context;
    
    EXPECT_FALSE(context.isValid());
    
    context.setRepository(repository_);
    EXPECT_FALSE(context.isValid());
    
    context.setSerializer(serializer_);
    EXPECT_FALSE(context.isValid());
    
    context.setView(view_);
    EXPECT_FALSE(context.isValid());
    
    context.setHistory(history_);
    EXPECT_FALSE(context.isValid());
}

TEST_F(CommandContextTest, IsValid_ReturnsTrue_WhenAllDependenciesSet) {
    CommandContext context;
    context.setRepository(repository_);
    context.setSerializer(serializer_);
    context.setView(view_);
    context.setHistory(history_);
    context.setRegistry(registry_.get());
    
    EXPECT_TRUE(context.isValid());
}

TEST_F(CommandContextTest, GetMissingDependencies_ReturnsAllMissing_WhenEmpty) {
    CommandContext context;
    
    std::string missing = context.getMissingDependencies();
    
    EXPECT_NE(missing.find("Repository"), std::string::npos);
    EXPECT_NE(missing.find("Serializer"), std::string::npos);
    EXPECT_NE(missing.find("View"), std::string::npos);
    EXPECT_NE(missing.find("History"), std::string::npos);
    EXPECT_NE(missing.find("Registry"), std::string::npos);
}

TEST_F(CommandContextTest, GetMissingDependencies_ReturnsEmpty_WhenAllSet) {
    CommandContext context;
    context.setRepository(repository_);
    context.setSerializer(serializer_);
    context.setView(view_);
    context.setHistory(history_);
    context.setRegistry(registry_.get());
    
    std::string missing = context.getMissingDependencies();
    
    EXPECT_TRUE(missing.empty());
}

TEST_F(CommandContextTest, GetHistoryTyped_ReturnsTypedPointer) {
    CommandContext context;
    context.setHistory(history_);
    
    auto typedHistory = context.getHistoryTyped();
    
    EXPECT_EQ(typedHistory, history_);
}

TEST_F(CommandContextTest, GetRegistryTyped_ReturnsTypedPointer) {
    CommandContext context;
    context.setRegistry(registry_.get());
    
    auto* typedRegistry = context.getRegistryTyped();
    
    EXPECT_EQ(typedRegistry, registry_.get());
}