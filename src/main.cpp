#include "Effect.hpp"
#include "config.hpp"
#include "defines.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>

f32 zoom = 1.f;
u8 idx = 0;
bool isRepulsing = false;

void drawFPS(sf::RenderWindow &window, sf::Font &font, f32 dt) {
    sf::Text txt;
    txt.setFont(font);
    txt.setString("/// " + std::to_string((int)std::round(1.0f / dt)) + " FPS ///");
    txt.setPosition(10, 10);
    txt.setCharacterSize(24);
    txt.setFillColor(sf::Color::White);
    window.draw(txt);
}

i32 main() {
    sf::RenderWindow window(sf::VideoMode(win_width, win_height), win_title);
    sf::View view(sf::FloatRect(0.f, 0.f, win_width, win_height));
    window.setFramerateLimit(targetFPS);

    sf::Font font;
    if (!font.loadFromFile(ASSETS_PATH "consolas.ttf")) {
        std::cerr << "[ERR] : Impossible de charger la police" << std::endl;
        exit(EXIT_FAILURE);
    }

    f32 dt;
    Effect effect;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::R:
                    effect.loadImage(imgPaths[idx]);
                    idx = (idx + 1) % imgPaths.size();
                    break;

                case sf::Keyboard::Add:
                    zoom *= (1.f - zoomStep);
                    view.zoom(1.f - zoomStep);
                    break;

                case sf::Keyboard::Subtract:
                    zoom *= (1.f + zoomStep);
                    view.zoom(1.f + zoomStep);
                    break;

                case sf::Keyboard::Escape:
                    return 0;

                default:
                    break;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                isRepulsing = true;
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                isRepulsing = false;
            }
        }

        window.clear(sf::Color::Black);

        dt = clock.restart().asSeconds();
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        window.setView(view);
        drawFPS(window, font, dt);
        if (isRepulsing) {
            effect.applyRepulsion(mousePos);
        }
        effect.update(dt);
        effect.draw(window);

        window.display();
    }

    return 0;
}
