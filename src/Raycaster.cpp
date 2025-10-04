#include "../include/Raycaster.h"
#include "../include/Util.h"
#include "../include/Map.h"
#include "../include/Player.h"
#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>
#include <cassert>

Raycaster::Raycaster(const Map& map, const Player& player, sf::RenderWindow& window, int scnWidth, int scnHeight)
    : m_map{ map }
    , m_player{ player }
    , m_window{ window }
    , m_scnWidth{ scnWidth }
    , m_scnHeight{ scnHeight }
    , m_fovRad{ 90.0f * PI / 180.0f }
    , m_camPlane{ player.dir().perpendicular() * std::fabs(std::tan(m_fovRad / 2.0f)) }
{
    assert(scnWidth > 0 && scnHeight > 0 && "Raycaster dimensions must be greater than 0\n");
    assert(unsigned(scnWidth) <= window.getSize().x && "Raycaster width exceeds that of window\n");
    assert(unsigned(scnHeight) <= window.getSize().y && "Raycaster height exceeds that of window\n");

    m_fpsText.setString("FPS: " + std::to_string(60));
    m_fpsText.setCharacterSize(24);
    m_fpsText.setFillColor(sf::Color::Red);
    m_fpsText.setStyle(sf::Text::Bold);   

    std::cout << "Raycaster intialized with FOV: " << m_fovRad * 180.0f / PI << " degrees\n";
}

void Raycaster::showFPS(const float deltaTime) {
    m_fpsText.setString("FPS: " + std::to_string(1 / deltaTime));
    m_window.draw(m_fpsText);
}

void Raycaster::drawSky() {
    sf::RectangleShape sky({ float(m_scnWidth), float(m_scnHeight / 2.0f) });
    sky.setFillColor(sf::Color(0, 186, 255, 255)); // light blue
    m_window.draw(sky);
}

void Raycaster::drawFloor() {
    sf::RectangleShape floor({ float(m_scnWidth), float(m_scnHeight / 2.0f) });
    floor.setFillColor(sf::Color::Black);
    floor.setPosition({ float(0.0f), float(m_scnHeight / 2.0f) });
    m_window.draw(floor);
}

void Raycaster::castRays() {
    for (int scnX = 0; scnX <= m_scnWidth; ++scnX) {
        float camX = 2 * scnX / static_cast<float>(m_scnWidth) - 1;

        ddaResult ray = applyDDA(camX);
        drawWallSlice(scnX, ray.perpWallDist, ray.hitCellValue, ray.hitSide);

        // map2D.drawRays2D(ray.rayStart, ray.rayEnd);  // find a way to conditionalize this
    }
}

Raycaster::ddaResult Raycaster::applyDDA(const float camX) {
    sf::Vector2f rayStart = m_player.pos();
    sf::Vector2f rayDir = (m_player.dir() + camX * m_camPlane).normalized();
    float rayAngle = rayDir.angle().wrapUnsigned().asRadians(); // [0, 2pi) rads

    float deltaDistX = std::fabs(MAP_CELLSIDE / std::cos(rayAngle));
    float deltaDistY = std::fabs(MAP_CELLSIDE / std::sin(rayAngle));

    int rayMapCellX = static_cast<int>(rayStart.x / MAP_CELLSIDE);
    int rayMapCellY = static_cast<int>(rayStart.y / MAP_CELLSIDE);

    int stepX, stepY;
    float sideDistX, sideDistY;

    if (rayDir.x > 0) {
        stepX = 1;
        sideDistX = std::fabs(((rayMapCellX + 1) * MAP_CELLSIDE - rayStart.x) / std::cos(rayAngle));
    }
    else {
        stepX = -1;
        sideDistX = std::fabs((rayStart.x - rayMapCellX * MAP_CELLSIDE) / std::cos(rayAngle));
    }

    if (rayDir.y > 0) {
        stepY = 1;
        sideDistY = std::fabs(((rayMapCellY + 1) * MAP_CELLSIDE - rayStart.y) / std::sin(rayAngle));
    }
    else {
        stepY = -1;
        sideDistY = std::fabs((rayStart.y - rayMapCellY * MAP_CELLSIDE) / std::sin(rayAngle));
    }

    WallSide hitSide;
    int hitCellValue = 0;
    bool hitWall = false;
    bool hitBoundary = false;

    // perform the actual DDA
    while (true) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            rayMapCellX += stepX;
            hitSide = WallSide::northSouth;
        }
        else {
            sideDistY += deltaDistY;
            rayMapCellY += stepY;
            hitSide = WallSide::eastWest;
        }

        if (rayMapCellX < 0 || rayMapCellX >= m_map.cellCountX() ||
            rayMapCellY < 0 || rayMapCellY >= m_map.cellCountY()) {
            hitBoundary = true;
            break;
        }

        hitCellValue = m_map.cellValue(rayMapCellX, rayMapCellY);
        if (hitCellValue > 0) {
            hitWall = true;
            break;
        }
    }
    
    /*
        // need a way of continuing the loop in outer function
        if (hitBoundary) 
            continue; 
    */

    float rayLength;
    if (hitSide == WallSide::northSouth)
        rayLength = sideDistX - deltaDistX;
    else
        rayLength = sideDistY - deltaDistY;

    sf::Vector2f rayEnd(rayStart + rayLength * rayDir);
    float rayPlayerAngle = std::fabs(rayAngle - m_player.angleDeg() * PI / 180.0f);
    float perpWallDist = std::fabs(rayLength * std::cos(rayPlayerAngle));

    return { rayStart, rayEnd, perpWallDist, hitSide, hitCellValue };
}

void Raycaster::drawWallSlice(const int scnX, const float perpWallDist, const int hitCellValue, const WallSide hitSide) {
    // Calculate the height of the line to be drawn on screen
    const float lineHeight = (MAP_CELLSIDE * m_scnHeight) / perpWallDist;

    // Calculate coordinate of lowest and highest pixel to draw
    sf::Vector2f drawStart(float(scnX), -lineHeight / 2.0f + m_scnHeight / 2.0f);
    if (drawStart.y < 0)
        drawStart.y = 0;

    sf::Vector2f drawEnd(float(scnX), lineHeight / 2.0f + m_scnHeight / 2.0f);
    if (drawEnd.y >= WINDOW_HEIGHT)
        drawEnd.y = m_scnHeight - 1;

    // Draw the wall strip corresponding to the current ray
    sf::Color wallColor = wallColors[hitCellValue];

    if (hitSide == WallSide::eastWest) // apply a darker shade for EW walls
        this->halfColor(wallColor);
    
    // std::cerr << "Wall color: "; debugPrintColor(wallColor);

    std::array wallStrip{
        sf::Vertex{ drawStart, wallColor }, sf::Vertex{ drawEnd, wallColor }
    };
    m_window.draw(wallStrip.data(), wallStrip.size(), sf::PrimitiveType::Lines);
}

void Raycaster::halfColor(sf::Color& color) {
    color.r /= 2;
    color.g /= 2;
    color.b /= 2;
}
