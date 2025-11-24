#include <gtest/gtest.h>
#include "model/SlideRepository.hpp"
#include "model/Slide.hpp"

using namespace slideEditor::model;

class SlideRepositoryTest : public ::testing::Test {
protected:
    SlideRepository repository;
    
    void SetUp() override {}
};

TEST_F(SlideRepositoryTest, Constructor_CreatesEmpty) {
    EXPECT_EQ(repository.getSlideCount(), 0);
}

TEST_F(SlideRepositoryTest, AddSlide_ReturnsId) {
    auto slide = std::make_unique<Slide>(0, "Title", "Content", "Theme");
    
    int id = repository.addSlide(std::move(slide));
    
    EXPECT_GT(id, 0);
    EXPECT_EQ(repository.getSlideCount(), 1);
}

TEST_F(SlideRepositoryTest, AddSlide_AutoIncrementsId) {
    auto slide1 = std::make_unique<Slide>(0, "Title1", "Content1", "Theme1");
    auto slide2 = std::make_unique<Slide>(0, "Title2", "Content2", "Theme2");
    
    int id1 = repository.addSlide(std::move(slide1));
    int id2 = repository.addSlide(std::move(slide2));
    
    EXPECT_EQ(id2, id1 + 1);
}

TEST_F(SlideRepositoryTest, AddSlide_ReturnsMinusOneForNull) {
    int id = repository.addSlide(nullptr);
    
    EXPECT_EQ(id, -1);
    EXPECT_EQ(repository.getSlideCount(), 0);
}

TEST_F(SlideRepositoryTest, GetSlide_ReturnsPointer) {
    auto slide = std::make_unique<Slide>(0, "Title", "Content", "Theme");
    int id = repository.addSlide(std::move(slide));
    
    auto* retrieved = repository.getSlide(id);
    
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->getTitle(), "Title");
}

TEST_F(SlideRepositoryTest, GetSlide_ReturnsNullForInvalidId) {
    auto* retrieved = repository.getSlide(999);
    
    EXPECT_EQ(retrieved, nullptr);
}

TEST_F(SlideRepositoryTest, RemoveSlide_DecreasesCount) {
    auto slide = std::make_unique<Slide>(0, "Title", "Content", "Theme");
    int id = repository.addSlide(std::move(slide));
    
    bool removed = repository.removeSlide(id);
    
    EXPECT_TRUE(removed);
    EXPECT_EQ(repository.getSlideCount(), 0);
}

TEST_F(SlideRepositoryTest, RemoveSlide_ReturnsFalseForInvalidId) {
    bool removed = repository.removeSlide(999);
    
    EXPECT_FALSE(removed);
}

TEST_F(SlideRepositoryTest, Exists_ReturnsTrueForValidId) {
    auto slide = std::make_unique<Slide>(0, "Title", "Content", "Theme");
    int id = repository.addSlide(std::move(slide));
    
    EXPECT_TRUE(repository.exists(id));
}

TEST_F(SlideRepositoryTest, Exists_ReturnsFalseForInvalidId) {
    EXPECT_FALSE(repository.exists(999));
}

TEST_F(SlideRepositoryTest, Clear_RemovesAllSlides) {
    repository.addSlide(std::make_unique<Slide>(0, "T1", "C1", "Th1"));
    repository.addSlide(std::make_unique<Slide>(0, "T2", "C2", "Th2"));
    repository.addSlide(std::make_unique<Slide>(0, "T3", "C3", "Th3"));
    
    repository.clear();
    
    EXPECT_EQ(repository.getSlideCount(), 0);
}

TEST_F(SlideRepositoryTest, GetAllSlides_ReturnsReference) {
    repository.addSlide(std::make_unique<Slide>(0, "T1", "C1", "Th1"));
    repository.addSlide(std::make_unique<Slide>(0, "T2", "C2", "Th2"));
    
    const auto& slides = repository.getAllSlides();
    
    EXPECT_EQ(slides.size(), 2);
}