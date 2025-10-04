#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "Util.h"
#include <SFML/Graphics.hpp>

class Player;
class Map;

enum class WallSide { northSouth, eastWest };

class Raycaster {
public:
    struct ddaResult {
        sf::Vector2f rayStart;
        sf::Vector2f rayEnd;
        float perpWallDist;
        WallSide hitSide;
        int hitCellValue;
    };

private:
    const Map& m_map;
    const Player& m_player;
    sf::RenderWindow& m_window;
    const int m_scnWidth;
    const int m_scnHeight;
    float m_fovRad;
    sf::Vector2f m_camPlane; // length = tan(fov/2)
    unsigned int m_fps; 
    sf::Text m_fpsText{ RENDER_FONT };

    static void halfColor(sf::Color& color);
    void drawSky();
    void drawFloor();
    void castRays();
    ddaResult applyDDA(const float camX);
    void drawWallSlice(const int scnX, const float perpWallDist, const int hitCellValue, const WallSide hitSide);

public:
    Raycaster(const Map& map, const Player& player, sf::RenderWindow& window, int scnWidth, int scnHeight);

    void setCamPlane(const sf::Vector2f& camPlane) { m_camPlane = camPlane; }
    void showFPS(const float deltaTime);

    void render() {
        drawSky();
        drawFloor();
        castRays();
    }
};

#endif
