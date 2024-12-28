#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class MovingPlatform {
public:
    MovingPlatform(const sf::Vector2f& pos1, const sf::Vector2f& pos2);
    void init();
    void update(float deltaTime, bool leverActivated);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    
private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    sf::Vector2f position1;
    sf::Vector2f position2;
    float moveProgress;
    static constexpr float MOVE_SPEED = 2.0f;  
};
