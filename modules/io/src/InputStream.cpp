#include "io/InputStream.hpp"
#include <sstream>

namespace slideEditor::io {

InputStream::InputStream(std::istream& stream)
    : stream_(&stream), ownsStream_(false), 
      lastChar_('\0'), hasUnget_(false) {}

InputStream::InputStream(const std::string& str)
    : ownsStream_(true), lastChar_('\0'), hasUnget_(false) {
    ownedStream_ = std::make_unique<std::istringstream>(str);
    stream_ = ownedStream_.get();
}

InputStream::~InputStream() {
    // Unique_ptr handles cleanup of owned stream
}

std::optional<char> InputStream::get() {
    if (hasUnget_) {
        hasUnget_ = false;
        return lastChar_;
    }
    
    if (!stream_ || stream_->eof()) {
        return std::nullopt;
    }
    
    char c;
    if (stream_->get(c)) {
        lastChar_ = c;
        return c;
    }
    
    return std::nullopt;
}

std::optional<char> InputStream::peek() {
    if (hasUnget_) {
        return lastChar_;
    }
    
    if (!stream_ || stream_->eof()) {
        return std::nullopt;
    }
    
    int peeked = stream_->peek();
    if (peeked == EOF) {
        return std::nullopt;
    }
    
    return static_cast<char>(peeked);
}

bool InputStream::eof() const {
    if (hasUnget_) {
        return false;
    }

    return !stream_ || stream_->eof();
}

bool InputStream::good() const {
    if (hasUnget_) {
        return true;
    }

    return stream_ && stream_->good();
}

void InputStream::unget() {
    hasUnget_ = true;
}

} // namespace slideEditor::io