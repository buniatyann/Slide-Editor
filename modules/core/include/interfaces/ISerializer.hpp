#ifndef I_SERIALIZER_HPP
#define I_SERIALIZER_HPP

#include <string>

namespace slideEditor::core {

class ISlideRepository;

class ISerializer {
public:
    virtual ~ISerializer() = default;
    virtual bool save(const ISlideRepository* repository, const std::string& filename) = 0;
    virtual bool load(ISlideRepository* repository, const std::string& filename) = 0;
    virtual std::string getLastError() const = 0;
};

} // namespace slideEditor::core

#endif // I_SERIALIZER_HPP