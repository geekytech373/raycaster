#ifndef MAP_H
#define MAP_H

#include "Util.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Map {
private:
    const int m_cellCountX{};
    const int m_cellCountY{};
    std::vector<int> m_array{}; // need to implement a robust compile-time check

public:
    Map(const int cellCountX, const int cellCountY, const std::vector<int>& arr);
    
    int cellCountX() const { return m_cellCountX; }
    int cellCountY() const { return m_cellCountY; }

    int cellValue(const int x, const int y) const { return m_array[y * m_cellCountX + x]; }
    void setCellValue(const int x, const int y, const int val) { m_array[y * m_cellCountX + x] = val; }
    
    int cellValue(const sf::Vector2f& pos) const {
        const int x = pos.x / MAP_CELLSIDE;
        const int y = pos.y / MAP_CELLSIDE;
        return m_array[y * m_cellCountX + x];
    }
};

#endif
