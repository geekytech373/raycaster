#include "../include/Player.h"
#include "../include/Map.h"
#include "../include/Raycaster.h"
#include "../include/Util.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

Player::Player()
    : m_pos{ 0.0f, 0.0f }
    , m_dir{ 1.0f, 1.0f }
    , m_movSpeed{ 2.0f }
    , m_rotSpeed{ 4.0f * PI / 180.0f }
{
    m_dir = m_dir.normalized();
    std::cout << "Player intialized\n";
}

bool Player::canMoveTo(const Map& map, const sf::Vector2f& pos) const {
    const float& r = PLAYER_RADIUS;
    const int minCellX = std::max(0, static_cast<int>(std::floor((pos.x - r) / MAP_CELLSIDE)));
    const int maxCellX = std::min(map.cellCountX() - 1, static_cast<int>(std::floor((pos.x + r) / MAP_CELLSIDE)));
    const int minCellY = std::max(0, static_cast<int>(std::floor((pos.y - r) / MAP_CELLSIDE)));
    const int maxCellY = std::min(map.cellCountY() - 1, static_cast<int>(std::floor((pos.y + r) / MAP_CELLSIDE)));

    for (int y = minCellY; y <= maxCellY; ++y) {
        for (int x = minCellX; x <= maxCellX; ++x) {
            int cellVal = map.cellValue(x, y);
            if (cellVal == 0) 
                continue; // empty

            // if the candidate circle intersects a non-empty cell, movement blocked
            if (doesCircleIntersectCell(pos, r, x, y))
                return false;
        }
    }

    return true;
}

void Player::handleInput(Map& map, Raycaster& raycaster) {
    sf::Vector2f forward = m_dir;
    sf::Vector2f delta = forward * m_movSpeed;

    // handle rotation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        m_dir = m_dir.rotatedBy(-sf::radians(m_rotSpeed));
        raycaster.setCamPlane(m_dir.perpendicular());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        m_dir = m_dir.rotatedBy(sf::radians(m_rotSpeed));
        raycaster.setCamPlane(m_dir.perpendicular());
    }

    // handle movement: compute desired motion, apply X then Y with collision checks
    sf::Vector2f moveDelta(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) 
        moveDelta += delta;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) 
        moveDelta -= delta;
    // optionally add strafing: left/right using perpendicular dir

    if (moveDelta.x != 0.f) {
        sf::Vector2f candidateX = m_pos + sf::Vector2f(moveDelta.x, 0.f);

        if (canMoveTo(map, candidateX))
            m_pos.x = candidateX.x;
        // else blocked along X, but we still try Y to slide
    }
    if (moveDelta.y != 0.f) {
        sf::Vector2f candidateY = m_pos + sf::Vector2f(0.f, moveDelta.y);

        if (canMoveTo(map, candidateY))
            m_pos.y = candidateY.y;
    }
}

void Player::placeInMapCell(const Map& map, const int x, const int y) {
    assert(x > 0 && x < map.cellCountX() && y > 0 && y < map.cellCountY()
        && "Cell is not within map!");
    assert(map.cellValue(x, y) == 0 && "Cannot place player on a non-empty cell!");

    m_pos.x = MAP_CELLSIDE / 2.0f + x * MAP_CELLSIDE;
    m_pos.y = MAP_CELLSIDE / 2.0f + y * MAP_CELLSIDE;

    std::cout << "Player placed at: (" << m_pos.x << ", " << m_pos.y << ")\n"; 
}
