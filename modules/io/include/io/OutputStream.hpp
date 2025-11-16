#ifndef OUTPUT_STREAM_HPP
#define OUTPUT_STREAM_HPP

#include "../../../core/include/interfaces/IOutputStream.hpp"
// #include " interfaces/IOutputStream.hpp"
#include <ostream>
#include <memory>

namespace slideEditor::io {

class OutputStream : public core::IOutputStream {
public:
    explicit OutputStream(std::ostream& stream);
    OutputStream();
    ~OutputStream() override;
    
    void write(const std::string& data) override;
    void writeLine(const std::string& line) override;
    void flush() override;
    bool good() const override;
    bool fail() const override;
    
    std::string getOutput() const;

private:
    std::unique_ptr<std::ostream> ownedStream_;
    std::ostream* stream_;
    bool ownsStream_;
};

} // namespace slideEditor::io

#endif // OUTPUT_STREAM_HPP