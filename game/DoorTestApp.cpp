#include <SFML/Graphics.hpp>
#include "Door.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Door Test");
    window.setFramerateLimit(60);
    
    Door door(DoorType::Water);  
    door.init();
    door.setPosition(sf::Vector2f(400, 300));  
    
    sf::Clock clock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (door.isOpen()) {
                    door.close();
                } else {
                    door.open();
                }
            }
            
            door.handleEvent(event);
        }
        
        float deltaTime = clock.restart().asSeconds();
        door.update(deltaTime);
        
        window.clear(sf::Color(100, 100, 100));  
        door.draw(window);
        window.display();
    }
    
    return 0;
}
