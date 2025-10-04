#include "../include/Map.h"
#include "../include/Player.h"
#include "../include/Raycaster.h"
#include "../include/Map2D.h"
#include "../include/MapData.h"
#include "../include/Util.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
                            "Raycaster", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(TARGET_FPS);

    Map map(24, 24, map2Data);
    Map2D map2D(map);
    Player player;
    player.placeInMapCell(map, 1, 1);
    Raycaster raycaster(map, player, window, WINDOW_HEIGHT, WINDOW_HEIGHT);

    sf::Clock clock;
    double frameStart, frameEnd;
    double deltaTime = TARGET_FPS;

    // ---------------------- Game Loop ---------------------------------------
    while (window.isOpen()) {
        frameStart = clock.getElapsedTime().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Press M to toggle map (hidden by default)
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::M) {
                    std::cout << "Map toggled\n";
                    map2D.toggleMap();
                }
            }

            // Press Q to quit
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Q) {
                    window.close();
                }
            }
        }

        player.handleInput(map, raycaster);
        window.clear(sf::Color(51, 51, 51));

        // ------------------- Drawing ----------------------------------------
        raycaster.render();

        if (map2D.isShown()) {
            map2D.drawGrid(window);
            map2D.drawPlayer(player, window);
        }

        raycaster.showFPS(deltaTime);
        window.display();

        // Calculate the FPS using the difference in the time elapsed between the last two frames
        frameEnd = clock.getElapsedTime().asSeconds();
        deltaTime = frameEnd - frameStart;
    }

    return 0;
}
