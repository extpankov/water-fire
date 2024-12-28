#include "PressureButton.h"

PressureButton::PressureButton() : pressed(false), currentOffset(0.0f) {
}

void PressureButton::init() {
    buttonTexture = std::make_shared<sf::Texture>();
    if (!buttonTexture->loadFromFile("data/buttons/leg_button.png")) {
        throw std::runtime_error("Failed to load button texture");
    }
    
    buttonSprite.setTexture(*buttonTexture);
    
    
    sf::Vector2u size = buttonTexture->getSize();
    buttonSprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
    
    
    buttonSprite.setOrigin(size.x / 2.0f, size.y - 3.0f);
}

void PressureButton::setPosition(const sf::Vector2f& pos) {
    position = pos;
    buttonSprite.setPosition(position);
}

void PressureButton::update(float deltaTime) {
    float targetOffset = pressed ? PRESS_OFFSET : 0.0f;
    
    
    if (currentOffset < targetOffset) {
        currentOffset += ANIMATION_SPEED * deltaTime;
        if (currentOffset > targetOffset) currentOffset = targetOffset;
    } else if (currentOffset > targetOffset) {
        currentOffset -= ANIMATION_SPEED * deltaTime;
        if (currentOffset < targetOffset) currentOffset = targetOffset;
    }
    
    
    sf::Vector2u size = buttonTexture->getSize();
    
    
    buttonSprite.setPosition(position + sf::Vector2f(0.0f, currentOffset));
    
    
    int pixelsToCrop = static_cast<int>(currentOffset);
    buttonSprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y - pixelsToCrop));
}

void PressureButton::draw(sf::RenderWindow& window) const {
    window.draw(buttonSprite);
}

bool PressureButton::contains(const sf::Vector2f& point) const {
    return buttonSprite.getGlobalBounds().contains(point);
}

void PressureButton::setPressed(bool value) {
    pressed = value;
}
