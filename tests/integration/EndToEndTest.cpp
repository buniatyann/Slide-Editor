#include <gtest/gtest.h>
#include "controller/CommandController.hpp"
#include "model/SlideRepository.hpp"
#include "serialization/JsonSerializer.hpp"
#include "view/cli/CliView.hpp"
#include "io/InputStream.hpp"
#include <sstream>
#include <memory>

using namespace slideEditor;

class EndToEndTest : public ::testing::Test {
protected:
    std::shared_ptr<model::SlideRepository> repository_;
    std::shared_ptr<serialization::JsonSerializer> serializer_;
    std::ostringstream viewOutput_;
    std::shared_ptr<view::CliView> view_;
    std::unique_ptr<controller::CommandController> controller_;
    
    void SetUp() override {
        repository_ = std::make_shared<model::SlideRepository>();
        serializer_ = std::make_shared<serialization::JsonSerializer>();
        view_ = std::make_shared<view::CliView>(viewOutput_);
        
        // Create controller with null input (we'll use processCommand directly)
        controller_ = std::make_unique<controller::CommandController>(
            repository_, serializer_, view_, nullptr);
    }
    
    bool executeCommand(const std::string& cmd) {
        viewOutput_.str("");  // Clear output
        viewOutput_.clear();
        
        // Use processCommand (public) instead of processCommandLine (private)
        return controller_->processCommand(cmd);
    }
    
    std::string getOutput() const {
        return viewOutput_.str();
    }
};

TEST_F(EndToEndTest, CreateSlide_Success) {
    bool result = executeCommand("create MyTitle MyContent MyTheme");
    
    EXPECT_TRUE(result);
    EXPECT_EQ(repository_->getSlideCount(), 1);
}

TEST_F(EndToEndTest, CreateAndDisplay_ShowsSlide) {
    executeCommand("create Title1 Content1 Theme1");
    executeCommand("display");
    
    std::string output = getOutput();
    EXPECT_NE(output.find("Title1"), std::string::npos);
}

TEST_F(EndToEndTest, AddShape_ToSlide) {
    executeCommand("create Title Content Theme");
    bool result = executeCommand("addshape 0 circle 1.5");
    
    EXPECT_TRUE(result);
}

TEST_F(EndToEndTest, SaveAndLoad_Roundtrip) {
    executeCommand("create Slide1 Content1 Theme1");
    executeCommand("save test_presentation.json");
    
    // Clear repository
    repository_ = std::make_shared<model::SlideRepository>();
    controller_ = std::make_unique<controller::CommandController>(
        repository_, serializer_, view_, nullptr);
    
    executeCommand("load test_presentation.json");
    
    EXPECT_EQ(repository_->getSlideCount(), 1);
}

TEST_F(EndToEndTest, HelpCommand_ShowsHelp) {
    executeCommand("help");
    
    std::string output = getOutput();
    EXPECT_FALSE(output.empty());
}

TEST_F(EndToEndTest, InvalidCommand_ShowsError) {
    executeCommand("invalidcommand");
    
    std::string output = getOutput();
    EXPECT_NE(output.find("ERROR"), std::string::npos);
}