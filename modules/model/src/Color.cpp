#include "model/Color.hpp"
#include <algorithm>
#include <cctype>

namespace slideEditor::model {

Color Color::fromString(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    // Named colors
    if (lower == "black") {
        return Black();
    }
    if (lower == "white") {
        return White();
    }
    if (lower == "red") {
        return Red();
    }
    if (lower == "green") {
        return Green();
    }
    if (lower == "blue") {
        return Blue();
    }
    if (lower == "yellow") {
        return Yellow();
    }
    if (lower == "cyan") {
        return Cyan();
    }
    if (lower == "magenta") {
        return Magenta();
    }
    if (lower == "orange") {
        return Orange();
    }
    if (lower == "purple") {
        return Purple();
    }
    if (lower == "gray" || lower == "grey") {
        return Gray();
    }
    if (lower == "transparent") {
        return Transparent();
    }
    
    // Hex format: #RRGGBB or #RGB
    if (str.length() > 0 && str[0] == '#') {
        std::string hex = str.substr(1);
        if (hex.length() == 3) {
            // Short format #RGB -> #RRGGBB
            hex = std::string(2, hex[0]) + std::string(2, hex[1]) + std::string(2, hex[2]);
        }
        
        if (hex.length() == 6) {
            try {
                unsigned long value = std::stoul(hex, nullptr, 16);
                unsigned char r = (value >> 16) & 0xFF;
                unsigned char g = (value >> 8) & 0xFF;
                unsigned char b = value & 0xFF;
                return Color(r, g, b);
            } catch (...) {
                return Black();  // Default on parse error
            }
        }
    }
    
    return Black();  // Default color
}

} // namespace slideEditor::model