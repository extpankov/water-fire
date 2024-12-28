#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(const sf::Vector2f& pos1, const sf::Vector2f& pos2)
    : position1(pos1), position2(pos2), moveProgress(0.0f) {
}

void MovingPlatform::init() {
    texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile("data/lever/platform.png")) {
        throw std::runtime_error("Failed to load platform texture");
    }
    
    sprite.setTexture(*texture);
    sprite.setPosition(position1);
}

void MovingPlatform::update(float deltaTime, bool leverActivated) {
    float targetProgress = leverActivated ? 1.0f : 0.0f;
    
    if (moveProgress < targetProgress) {
        moveProgress += MOVE_SPEED * deltaTime;
        if (moveProgress > targetProgress) moveProgress = targetProgress;
    } else if (moveProgress > targetProgress) {
        moveProgress -= MOVE_SPEED * deltaTime;
        if (moveProgress < targetProgress) moveProgress = targetProgress;
    }
    
    sf::Vector2f currentPos = position1 + (position2 - position1) * moveProgress;
    sprite.setPosition(currentPos);
}

void MovingPlatform::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::FloatRect MovingPlatform::getBounds() const {
    return sprite.getGlobalBounds();
}
