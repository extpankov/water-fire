#include <SFML/Graphics.hpp>
#include "Water.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Water Character Test");
    window.setFramerateLimit(60);

    Water water;
    water.init();
    
    
    water.setPosition(sf::Vector2f(400, 300));
    
    
    
    
    
    water.setRunning();
    
    sf::Clock clock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        float deltaTime = clock.restart().asSeconds();
        
        
        water.update(deltaTime);
        
        
        window.clear(sf::Color(128, 128, 128));
        
        
        water.draw(window);
        
        window.display();
    }
    
    return 0;
}
