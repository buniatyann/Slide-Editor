#ifndef I_OUTPUT_STREAM_HPP
#define I_OUTPUT_STREAM_HPP

#include <string>

namespace slideEditor::core {

class IOutputStream {
public:
    virtual ~IOutputStream() = default;
    
    virtual void write(const std::string& data) = 0;
    virtual void writeLine(const std::string& line) = 0;
    virtual void flush() = 0;
    
    virtual bool good() const = 0;
    virtual bool fail() const = 0;
};

} // namespace slideEditor::core

#endif // I_OUTPUT_STREAM_HPP