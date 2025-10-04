#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Map;
class Raycaster;

class Player {
private:
    sf::Vector2f m_pos;   // world position
    sf::Vector2f m_dir;   // direction vector (unit vector)
    float m_movSpeed;     // game units per second
    float m_rotSpeed;     // radians per sceond

public:
    Player();

    const sf::Vector2f& pos() const { return m_pos; }
    const sf::Vector2f& dir() const { return m_dir; }
    float movSpeed() const { return m_movSpeed; }
    float rotSpeed() const { return m_rotSpeed; }
    float angleDeg() const { return m_dir.angle().wrapUnsigned().asDegrees(); }

    bool canMoveTo(const Map& map, const sf::Vector2f& pos) const;
    void placeInMapCell(const Map& map, const int x, const int y);
    void handleInput(Map& map, Raycaster& raycaster);
};

#endif
