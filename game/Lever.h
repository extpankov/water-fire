#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Lever {
public:
    Lever();
    void init();
    void setPosition(const sf::Vector2f& pos);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    void toggle();
    bool contains(const sf::Vector2f& point) const;
    bool isActivated() const;
    void resetTrigger() { wasTriggered = false; }
    bool wasPreviouslyTriggered() const { return wasTriggered || cooldownTimer > 0.0f; }
    void setTriggered();
    bool checkAndUpdateApproach(const sf::Vector2f& characterPos);
    
private:
    std::shared_ptr<sf::Texture> baseTexture;
    std::shared_ptr<sf::Texture> handleTexture;
    sf::Sprite baseSprite;
    sf::Sprite handleSprite;
    sf::Vector2f position;
    sf::Vector2f lastCharacterPos;
    bool hasLastCharacterPos;
    
    bool isOn;
    bool isAnimating;
    float animationProgress;
    bool wasTriggered;
    float cooldownTimer;  
    static constexpr float COOLDOWN_DURATION = 1.0f;  
};
