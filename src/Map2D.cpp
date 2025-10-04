#include "../include/Map2D.h"
#include "../include/Map.h"
#include "../include/Player.h"
#include "../include/Util.h"

#include <iostream>

Map2D::Map2D(const Map& map)
    : m_map{ map }
    , m_drawSize{ WINDOW_WIDTH - WINDOW_HEIGHT, WINDOW_WIDTH - WINDOW_HEIGHT }
    , m_drawOffset{ WINDOW_HEIGHT, 0 }
    , m_cellSize{ m_drawSize.x / m_map.cellCountX() }
    , m_scale{ m_drawSize.x / (m_map.cellCountX() * MAP_CELLSIDE ) }
    , m_isShown{ false }
{ 
    std::cout << "2D Map created with size: " << m_drawSize.x << " X " << m_drawSize.y << '\n'; 
}

void Map2D::drawGrid(sf::RenderWindow& window) {
    sf::RectangleShape tile({ m_cellSize - 1.0f, m_cellSize - 1.0f });

    for (int y = 0; y < m_map.cellCountY(); ++y) {
        for (int x = 0; x < m_map.cellCountX(); ++x) {
            int tileValue = m_map.cellValue(x, y);
            tile.setFillColor(wallColors[tileValue]);
            tile.setPosition({ m_drawOffset.x + x * m_cellSize + 1.0f, m_drawOffset.y + y * m_cellSize + 1.0f });
            window.draw(tile);
        }
    }
}

void Map2D::drawPlayer(const Player& p, sf::RenderWindow& window) {
    float iconRadius = std::max(8.0f, m_cellSize / 5);
    sf::CircleShape icon(iconRadius);
    icon.setFillColor(sf::Color::Yellow);

    icon.setOrigin(icon.getLocalBounds().size * 0.5f);
    icon.setPosition(getScnCoords(p.pos()));
    window.draw(icon);

    // std::cerr << "Player icon screen coordinates: (" << icon.getPosition().x << ", " << icon.getPosition().y << ")\n";

    // Draw player direction line
    std::array dirLine{
        sf::Vertex{ getScnCoords(p.pos())                              , sf::Color::Red },
        sf::Vertex{ getScnCoords(p.pos()) + 2.0f * iconRadius * p.dir(), sf::Color::Red }
    };
    window.draw(dirLine.data(), dirLine.size(), sf::PrimitiveType::Lines);
}
