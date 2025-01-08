#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class PushableBlock {
public:
    PushableBlock();
    void init();
    void setPosition(const sf::Vector2f& pos);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    void applyForce(const sf::Vector2f& force, float deltaTime);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getVelocity() const { return velocity; }
    void setVelocity(const sf::Vector2f& vel) { velocity = vel; }
    void setOnGround(bool grounded);
    
    static constexpr float getPushForce() { return PUSH_FORCE; }
    
private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool onGround;
    
    static constexpr float FRICTION = 3.0f;  
    static constexpr float MAX_VELOCITY = 150.0f;  
    static constexpr float GRAVITY = 800.0f;  
public:
    static constexpr float PUSH_FORCE = 500.0f;  
};
