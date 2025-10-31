#ifndef JSON_SERIALIZER_HPP
#define JSON_SERIALIZER_HPP

#include "interfaces/ISerializer.hpp"
#include "interfaces/ISlideRepository.hpp"
#include <string>

namespace slideEditor::serialization {

class JsonSerializer : public core::ISerializer {
public:
    JsonSerializer() = default;
    
    bool save(const core::ISlideRepository* repository, 
              const std::string& filename) override;
    
    bool load(core::ISlideRepository* repository, 
              const std::string& filename) override;
    
    std::string getLastError() const override;

private:
    std::string lastError_;
    
    std::string serializeRepository(const core::ISlideRepository* repo);
    bool deserializeToRepository(core::ISlideRepository* repo, 
                                  const std::string& jsonStr);
};

} // namespace slideEditor::serialization

#endif // JSON_SERIALIZER_HPP