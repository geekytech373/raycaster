#include "../include/Map.h"

#include <iostream>
#include <cassert>
#include <vector>

Map::Map(const int cellCountX, const int cellCountY, const std::vector<int>& arr)
    : m_cellCountX{ cellCountX }
    , m_cellCountY{ cellCountY }
    , m_array{ arr }
{
    assert(m_array.size() == static_cast<size_t>(m_cellCountX * m_cellCountY)
           && "Map dimensions don't match\n");

    std::cout << "Map loaded from data with width: " << m_cellCountX * MAP_CELLSIDE;
    std::cout << ", height: " << m_cellCountY * MAP_CELLSIDE << "\n"; 
}
