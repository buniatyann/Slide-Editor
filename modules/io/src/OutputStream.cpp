// #include "io/OutputStream.hpp"
#include "../include/io/OutputStream.hpp"
#include <sstream>

namespace slideEditor::io {

OutputStream::OutputStream(std::ostream& stream)
    : stream_(&stream), ownsStream_(false) {}

OutputStream::OutputStream()
    : ownsStream_(true) {
    ownedStream_ = std::make_unique<std::ostringstream>();
    stream_ = ownedStream_.get();
}

OutputStream::~OutputStream() {}

void OutputStream::write(const std::string& data) {
    if (stream_) {
        *stream_ << data;
    }
}

void OutputStream::writeLine(const std::string& line) {
    if (stream_) {
        *stream_ << line << '\n';
    }
}

void OutputStream::flush() {
    if (stream_) {
        stream_->flush();
    }
}

bool OutputStream::good() const {
    return stream_ && stream_->good();
}

bool OutputStream::fail() const {
    return !stream_ || stream_->fail();
}

std::string OutputStream::getOutput() const {
    if (auto* sstream = dynamic_cast<std::ostringstream*>(stream_)) {
        return sstream->str();
    }

    return "";
}

} // namespace slideEditor::io