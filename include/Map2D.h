#ifndef MAP2D_H
#define MAP2D_H

#include <SFML/Graphics.hpp>

class Map;
class Player;

class Map2D {
private:
    const Map& m_map;
    sf::Vector2f m_drawSize;
    sf::Vector2f m_drawOffset;
    float m_cellSize;
    float m_scale;
    bool m_isShown;
    
public:
    Map2D(const Map& map);

    void toggleMap() { m_isShown = !m_isShown; }
    bool isShown() const { return m_isShown; }

    void drawGrid(sf::RenderWindow& window);
    void drawPlayer(const Player& p, sf::RenderWindow& window);
    void drawRays();

    sf::Vector2f getScnCoords(const sf::Vector2f& gameCoords) { 
        return gameCoords * m_scale + m_drawOffset;
    }
};

#endif
