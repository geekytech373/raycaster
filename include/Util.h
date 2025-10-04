#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>
#include <array>

constexpr int WINDOW_WIDTH  = 1600;
constexpr int WINDOW_HEIGHT = 900;
constexpr int MAP_CELLSIDE  = 100; // game units
constexpr float PI          = 3.14159f;
constexpr float PLAYER_RADIUS = 18.0f;
constexpr unsigned int TARGET_FPS = 60u;
inline const sf::Font RENDER_FONT("/usr/share/fonts/truetype/ubuntu/UbuntuSansMono[wght].ttf");   

// Color palette for map tiles / walls (index 0 = empty)
constexpr std::array<sf::Color, 6> wallColors = { 
    sf::Color::Black, sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Magenta
};

// Helper functions
float clampf(const float val, const float low, const float high);
inline float degToRad(const float deg) { return deg * (PI / 180.0f); }
inline float radToDeg(const float rad) { return rad * (180.0f / PI); }

bool doesCircleIntersectCell(const sf::Vector2f& centre, const float r, const int cellX, const int cellY);

// Debug printing (no-op when NDEBUG is defined)
void debugPrintColor(const sf::Color& c);
void debugPrintVec2f(const sf::Vector2f& v);

#endif
