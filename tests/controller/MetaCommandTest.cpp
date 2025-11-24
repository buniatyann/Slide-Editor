#include <gtest/gtest.h>

#include "controller/MetaCommand.hpp"
#include "interfaces/ICommand.hpp"

#include <memory>
#include <string>
#include <vector>

using slideEditor::controller::MetaCommand;
using slideEditor::core::ArgumentInfo;
using slideEditor::core::CommandCreator;
using slideEditor::core::ICommand;

namespace {

std::unique_ptr<MetaCommand> makeMetaCommand(
    CommandCreator creator = CommandCreator{},
    std::initializer_list<ArgumentInfo> arguments = {}) {

    if (!creator) {
        creator = [](const std::vector<std::string>&, void*) {
            return std::unique_ptr<ICommand>();
        };
    }

    return std::make_unique<MetaCommand>(
        "create",
        "Create a new slide",
        "ACTION",
        std::move(creator),
        arguments);
}

}  // namespace

class MetaCommandTest : public ::testing::Test {};

TEST_F(MetaCommandTest, ProvidesMetadataAndCallableCreator) {
    bool invoked = false;
    auto meta = makeMetaCommand(
        [&invoked](const std::vector<std::string>& args, void*) {
            invoked = true;
            EXPECT_TRUE(args.empty());
            return std::unique_ptr<ICommand>();
        });

    EXPECT_EQ(meta->getName(), "create");
    EXPECT_EQ(meta->getDescription(), "Create a new slide");
    EXPECT_EQ(meta->getCategory(), "ACTION");

    auto creator = meta->getCreator();
    creator({}, nullptr);
    EXPECT_TRUE(invoked);
}

TEST_F(MetaCommandTest, CountsRequiredAndMaximumArguments) {
    auto meta = makeMetaCommand(
        CommandCreator{},
        { ArgumentInfo{"title", "string", "Slide title"},
          ArgumentInfo{"theme", "string", "Visual theme", false},
          ArgumentInfo{"notes", "string", "Speaker notes", false} });

    EXPECT_EQ(meta->getRequiredArgCount(), 1u);
    EXPECT_EQ(meta->getMaxArgCount(), 3u);
}

TEST_F(MetaCommandTest, ValidateArgumentsChecksCountAndTypes) {
    auto meta = makeMetaCommand(
        CommandCreator{},
        { ArgumentInfo{"id", "int", "Slide identifier"},
          ArgumentInfo{"scale", "double", "Scaling factor", false} });

    EXPECT_TRUE(meta->validateArguments({"5"}));
    EXPECT_TRUE(meta->validateArguments({"5", "1.25"}));
    EXPECT_FALSE(meta->validateArguments({}));                    // missing required
    EXPECT_FALSE(meta->validateArguments({"not-int"}));           // wrong type
    EXPECT_FALSE(meta->validateArguments({"5", "bad", "extra"})); // too many
}

TEST_F(MetaCommandTest, ValidateArgumentsAllowsSkippingOptionalArgs) {
    auto meta = makeMetaCommand(
        CommandCreator{},
        { ArgumentInfo{"title", "string", "Slide title"},
          ArgumentInfo{"theme", "string", "Visual theme", false} });

    EXPECT_TRUE(meta->validateArguments({"Agenda"}));
}

TEST_F(MetaCommandTest, GetUsageFormatsRequiredAndOptionalArguments) {
    auto meta = makeMetaCommand(
        CommandCreator{},
        { ArgumentInfo{"title", "string", "Slide title"},
          ArgumentInfo{"theme", "string", "Visual theme", false} });

    EXPECT_EQ(meta->getUsage(), "create <title> [theme]");
}

TEST_F(MetaCommandTest, DetailedHelpIncludesArgumentsAndCategory) {
    auto meta = makeMetaCommand(
        CommandCreator{},
        { ArgumentInfo{"title", "string", "Slide title"},
          ArgumentInfo{"id", "int", "Slide id"} });

    auto help = meta->getDetailedHelp();
    EXPECT_NE(help.find("create <title> <id>"), std::string::npos);
    EXPECT_NE(help.find("Slide title"), std::string::npos);
    EXPECT_NE(help.find("Category: ACTION"), std::string::npos);
}