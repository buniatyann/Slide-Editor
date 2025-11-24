#include <gtest/gtest.h>

#include "controller/CommandController.hpp"
#include "io/InputStream.hpp"
#include "model/SlideRepository.hpp"
#include "serialization/JsonSerializer.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/ISlide.hpp"

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace {

class RecordingView : public slideEditor::core::IView {
public:
    struct DisplaySnapshot {
        size_t slideCount{};
        std::vector<std::string> titles;
    };

    void displayMessage(const std::string& message) override {
        messages.push_back(message);
    }

    void displayError(const std::string& error) override {
        errors.push_back(error);
    }

    void displaySlides(const slideEditor::core::ISlideRepository* repository) override {
        DisplaySnapshot snapshot;
        if (repository) {
            snapshot.slideCount = repository->getSlideCount();
            const auto& slides = repository->getAllSlides();
            for (const auto& slide : slides) {
                snapshot.titles.push_back(slide->getTitle());
            }
        }
        displays.push_back(snapshot);
    }

    void displayHelp(const std::string& helpText) override {
        helpOutputs.push_back(helpText);
    }

    void displayPrompt() override {
        ++promptCount;
    }

    std::vector<std::string> messages;
    std::vector<std::string> errors;
    std::vector<std::string> helpOutputs;
    std::vector<DisplaySnapshot> displays;
    size_t promptCount = 0;
};

}  // namespace

class EndToEndTest : public ::testing::Test {
protected:
    void SetUp() override {
        repository = std::make_shared<slideEditor::model::SlideRepository>();
        serializer = std::make_shared<slideEditor::serialization::JsonSerializer>();
        view = std::make_shared<RecordingView>();
        input = std::make_shared<slideEditor::io::InputStream>("");

        controller = std::make_unique<slideEditor::controller::CommandController>(
            repository,
            serializer,
            view,
            input);
    }

    bool run(const std::string& commandLine) {
        return controller->processCommand(commandLine);
    }

    std::shared_ptr<slideEditor::model::SlideRepository> repository;
    std::shared_ptr<slideEditor::serialization::JsonSerializer> serializer;
    std::shared_ptr<RecordingView> view;
    std::shared_ptr<slideEditor::io::InputStream> input;
    std::unique_ptr<slideEditor::controller::CommandController> controller;
};

TEST_F(EndToEndTest, ExecutesActionsAndHistoryCommands) {
    run("create Intro Content Theme");
    ASSERT_EQ(repository->getSlideCount(), 1u);

    run("addshape 1 circle 1.5 red blue");
    auto* slide = repository->getSlide(1);
    ASSERT_NE(slide, nullptr);
    EXPECT_EQ(slide->getShapeCount(), 1u);

    run("display");
    ASSERT_FALSE(view->displays.empty());
    const auto& snapshot = view->displays.back();
    EXPECT_EQ(snapshot.slideCount, 1u);
    EXPECT_NE(std::find(snapshot.titles.begin(), snapshot.titles.end(), "Intro"),
              snapshot.titles.end());

    run("undo");
    slide = repository->getSlide(1);
    ASSERT_NE(slide, nullptr);
    EXPECT_EQ(slide->getShapeCount(), 0u);

    run("redo");
    slide = repository->getSlide(1);
    ASSERT_NE(slide, nullptr);
    EXPECT_EQ(slide->getShapeCount(), 1u);
}

TEST_F(EndToEndTest, SaveAndLoadRoundTripPersistsSlides) {
    run("create Overview Content Theme");
    run("addshape 1 rectangle 2 green yellow");

    const std::string filename = "endtoend_autosave";
    std::filesystem::path filepath = std::filesystem::current_path() / filename;
    std::error_code ec;
    std::filesystem::remove(filepath, ec);

    run("save " + filename);
    ASSERT_TRUE(std::filesystem::exists(filepath));
    ASSERT_GT(std::filesystem::file_size(filepath), 0u);

    repository->clear();
    ASSERT_EQ(repository->getSlideCount(), 0u);

    run("load " + filename);
    ASSERT_EQ(repository->getSlideCount(), 1u);
    auto* slide = repository->getSlide(1);
    ASSERT_NE(slide, nullptr);
    EXPECT_EQ(slide->getShapeCount(), 1u);

    std::filesystem::remove(filepath, ec);
}