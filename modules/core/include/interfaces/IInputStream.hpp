#ifndef I_INPUT_STREAM_HPP
#define I_INPUT_STREAM_HPP

#include <optional>

namespace slideEditor::core {

class IInputStream {
public:
    virtual ~IInputStream() = default;
    virtual std::optional<char> get() = 0;
    virtual std::optional<char> peek() = 0;
    virtual bool eof() const = 0;
    virtual bool good() const = 0;
    
    // Position control (optional for some implementations)
    virtual void unget() = 0;
};

} // namespace slideEditor::core

#endif // I_INPUT_STREAM_HPP