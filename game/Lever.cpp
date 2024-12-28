#include "Lever.h"
#include <cmath>

Lever::Lever() : isOn(false), isAnimating(false), animationProgress(0.0f), wasTriggered(false), 
                 cooldownTimer(0.0f), hasLastCharacterPos(false) {
}

void Lever::init() {
    baseTexture = std::make_shared<sf::Texture>();
    handleTexture = std::make_shared<sf::Texture>();
    
    if (!baseTexture->loadFromFile("data/lever/lever_base.png")) {
        throw std::runtime_error("Failed to load lever base texture");
    }
    if (!handleTexture->loadFromFile("data/lever/lever_handle.png")) {
        throw std::runtime_error("Failed to load lever handle texture");
    }
    
    baseSprite.setTexture(*baseTexture);
    handleSprite.setTexture(*handleTexture);
    
    
    sf::Vector2u handleSize = handleTexture->getSize();
    handleSprite.setOrigin(handleSize.x / 2.0f, handleSize.y - 5.0f);
    
    
    handleSprite.setRotation(45.0f);
}

bool Lever::checkAndUpdateApproach(const sf::Vector2f& characterPos) {
    bool shouldTrigger = false;
    
    if (!hasLastCharacterPos) {
        
        lastCharacterPos = characterPos;
        hasLastCharacterPos = true;
    } else {
        
        float leverCenterX = position.x;
        float characterX = characterPos.x;
        float lastCharacterX = lastCharacterPos.x;
        
        
        shouldTrigger = lastCharacterX > leverCenterX && characterX <= leverCenterX;
        
        lastCharacterPos = characterPos;
    }
    
    return shouldTrigger;
}

void Lever::setPosition(const sf::Vector2f& pos) {
    position = pos;
    baseSprite.setPosition(pos + sf::Vector2f(0.0f, -30.0f));
    handleSprite.setPosition(pos + sf::Vector2f(45.0f, 20.0f));
}

void Lever::update(float deltaTime) {
    if (cooldownTimer > 0.0f) {
        cooldownTimer -= deltaTime;
        if (cooldownTimer <= 0.0f) {
            cooldownTimer = 0.0f;
            wasTriggered = false;
            hasLastCharacterPos = false;  
        }
    }

    if (isAnimating) {
        animationProgress += deltaTime * 5.0f;
        if (animationProgress >= 1.0f) {
            animationProgress = 1.0f;
            isAnimating = false;
        }
        
        float startAngle = isOn ? 45.0f : -45.0f;  
        float endAngle = isOn ? -45.0f : 45.0f;    
        float currentAngle = startAngle + (endAngle - startAngle) * animationProgress;
        handleSprite.setRotation(currentAngle);
    }
}

void Lever::draw(sf::RenderWindow& window) const {
    window.draw(handleSprite);
    window.draw(baseSprite);
}

void Lever::toggle() {
    isOn = !isOn;
    isAnimating = true;
    animationProgress = 0.0f;
}

void Lever::setTriggered() {
    wasTriggered = true;
    cooldownTimer = COOLDOWN_DURATION;
}

bool Lever::contains(const sf::Vector2f& point) const {
    
    sf::FloatRect bounds = baseSprite.getGlobalBounds();
    bounds.left -= 5.0f;
    bounds.width += 10.0f;
    bounds.height += 10.0f;
    return bounds.contains(point);
}

bool Lever::isActivated() const {
    
    if (isAnimating) {
        return isOn && animationProgress >= 1.0f;
    }
    return isOn;
}
