#ifndef INPUT_STREAM_HPP
#define INPUT_STREAM_HPP

#include "interfaces/IInputStream.hpp"
#include <istream>
#include <string>
#include <memory>

namespace slideEditor::io {

class InputStream : public core::IInputStream {
public:
    explicit InputStream(std::istream& stream);
    explicit InputStream(const std::string& str);
    ~InputStream() override;
    
    std::optional<char> get() override;
    std::optional<char> peek() override;
    bool eof() const override;
    bool good() const override;
    void unget() override;

private:
    std::istream* stream_;
    bool ownsStream_;
    char lastChar_;
    bool hasUnget_;

    std::unique_ptr<std::istream> ownedStream_;
};

} // namespace slideEditor::io

#endif // INPUT_STREAM_HPP