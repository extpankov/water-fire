#include <SFML/Graphics.hpp>
#include "Lever.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Lever Demo");
    window.setFramerateLimit(60);
    
    Lever lever;
    lever.init();
    
    sf::Clock clock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    lever.toggle();
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    lever.toggle();
                }
            }
        }
        
        float deltaTime = clock.restart().asSeconds();
        lever.update(deltaTime);
        
        window.clear(sf::Color(30, 30, 30));
        lever.draw(window);
        window.display();
    }
    
    return 0;
}
