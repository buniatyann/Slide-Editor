#ifndef COLOR_HPP
#define COLOR_HPP
#include <string>
#include <sstream>
#include <iomanip>

namespace slideEditor::model {

/**
 * Color - Represents RGB color with alpha channel
 */
struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;  // transparency
    
    Color() : r(0), g(0), b(0), a(255) {}
    
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
        : r(r), g(g), b(b), a(a) {}
    
    static Color Black() { return Color(0, 0, 0); }
    static Color White() { return Color(255, 255, 255); }
    static Color Red() { return Color(255, 0, 0); }
    static Color Green() { return Color(0, 255, 0); }
    static Color Blue() { return Color(0, 0, 255); }
    static Color Yellow() { return Color(255, 255, 0); }
    static Color Cyan() { return Color(0, 255, 255); }
    static Color Magenta() { return Color(255, 0, 255); }
    static Color Orange() { return Color(255, 165, 0); }
    static Color Purple() { return Color(128, 0, 128); }
    static Color Gray() { return Color(128, 128, 128); }
    static Color Transparent() { return Color(0, 0, 0, 0); }
    
    // Convert to hex string for SVG
    std::string toHex() const {
        std::ostringstream oss;
        oss << "#" 
            << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(r)
            << std::setw(2) << static_cast<int>(g)
            << std::setw(2) << static_cast<int>(b);
        
        return oss.str();
    }
    
    // Get opacity (0.0 to 1.0)
    double getOpacity() const {
        return a / 255.0;
    }
    
    // Parse from string (e.g., "red", "FF0000")
    static Color fromString(const std::string& str);
};

} // namespace slideEditor::model

#endif // COLOR_HPP