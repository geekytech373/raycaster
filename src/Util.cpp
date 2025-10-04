#include "../include/Util.h"

#include <iostream>
#include <cmath>
#include <iomanip>

float clampf(const float val, const float low, const float high) {
    if (val < low) return low;
    if (val > high) return high;
    return val;
}

// returns true if circle centered at circlePos with radius r intersects the cell at (cellX,cellY)
bool doesCircleIntersectCell(const sf::Vector2f& centre, const float r, const int cellX, const int cellY) {
    const float cellLeft   = cellX * MAP_CELLSIDE;
    const float cellTop    = cellY * MAP_CELLSIDE;
    const float cellRight  = cellLeft + MAP_CELLSIDE;
    const float cellBottom = cellTop  + MAP_CELLSIDE;

    // Closest point on AABB to circle center
    const float closestX = std::max(cellLeft, std::min(centre.x, cellRight));
    const float closestY = std::max(cellTop,  std::min(centre.y, cellBottom));

    const float dx = centre.x - closestX;
    const float dy = centre.y - closestY;

    return (dx * dx + dy * dy) < (r * r);
}

void debugPrintColor(const sf::Color& c) {
#ifndef NDEBUG
    std::cerr << "Color (r,g,b,a): "
              << std::setw(3) << static_cast<int>(c.r) << ", "
              << std::setw(3) << static_cast<int>(c.g) << ", "
              << std::setw(3) << static_cast<int>(c.b) << ", "
              << std::setw(3) << static_cast<int>(c.a) << '\n';
#endif
}

void debugPrintVec2f(const sf::Vector2f& v) {
#ifndef NDEBUG
    std::cerr << "Vec2f (x,y): "
              << std::fixed << std::setprecision(3)
              << v.x << ", " << v.y << '\n';
#endif
}
