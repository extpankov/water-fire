#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class PressureButton {
public:
    PressureButton();
    void init();
    void setPosition(const sf::Vector2f& pos);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool contains(const sf::Vector2f& point) const;
    bool isPressed() const { return pressed; }
    void setPressed(bool value);
    
private:
    std::shared_ptr<sf::Texture> buttonTexture;
    sf::Sprite buttonSprite;
    sf::Vector2f position;
    bool pressed;
    
    static constexpr float PRESS_OFFSET = 5.0f;  
    float currentOffset;
    static constexpr float ANIMATION_SPEED = 10.0f;  
};
