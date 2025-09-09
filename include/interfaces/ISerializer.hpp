#ifndef SLIDEEDITOR_INTERFACES_ISERIALIZER_HPP
#define SLIDEEDITOR_INTERFACES_ISERIALIZER_HPP

#include <string>
#include "ISlideRepository.hpp"
#include "../core/SlideFactory.hpp"

class ISerializer {
public:
    virtual ~ISerializer() = default;
    virtual void save(const ISlideRepository& repo, const std::string& filepath) const = 0;
    virtual void load(ISlideRepository& repo, SlideFactory& factory, const std::string& filepath) = 0;
};

#endif // SLIDEEDITOR_INTERFACES_ISERIALIZER_HPP