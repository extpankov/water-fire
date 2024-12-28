#include "PushableBlock.h"
#include <cmath>
#include <iostream>

PushableBlock::PushableBlock() : velocity(0.0f, 0.0f), onGround(false) {
}

void PushableBlock::init() {
    texture = std::make_shared<sf::Texture>();
    std::cout << "Loading block texture..." << std::endl;
    if (!texture->loadFromFile("data/block.png")) {
        std::cerr << "Failed to load block texture" << std::endl;
        throw std::runtime_error("Failed to load block texture");
    }
    std::cout << "Block texture loaded successfully" << std::endl;
    
    sprite.setTexture(*texture);
    
    
    sf::Vector2u size = texture->getSize();
    std::cout << "Block texture size: " << size.x << "x" << size.y << std::endl;
    sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
    
    
    sprite.setScale(1.0f, 1.0f);
    sprite.setColor(sf::Color(255, 255, 255, 255)); 
}

void PushableBlock::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(position);
}

void PushableBlock::update(float deltaTime) {
    onGround = false;  
    
    if (!onGround) {
        velocity.y += GRAVITY * deltaTime;
    }
    
    
    float speed = std::abs(velocity.x);
    if (speed > 0) {
        float frictionForce = FRICTION * (onGround ? 1.0f : 0.2f); 
        if (speed < frictionForce * deltaTime) {
            velocity.x = 0;
        } else {
            velocity.x += (velocity.x > 0 ? -1 : 1) * frictionForce * deltaTime;
        }
    }
    
    
    if (velocity.y > MAX_VELOCITY) velocity.y = MAX_VELOCITY;
    if (velocity.y < -MAX_VELOCITY) velocity.y = -MAX_VELOCITY;
    if (velocity.x > MAX_VELOCITY) velocity.x = MAX_VELOCITY;
    if (velocity.x < -MAX_VELOCITY) velocity.x = -MAX_VELOCITY;
    
    
    position += velocity * deltaTime;
    sprite.setPosition(position);
}

void PushableBlock::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void PushableBlock::applyForce(const sf::Vector2f& force, float deltaTime) {
    velocity += force * deltaTime;
    
    
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > MAX_VELOCITY) {
        velocity = velocity / speed * MAX_VELOCITY;
    }
}

sf::FloatRect PushableBlock::getBounds() const {
    return sprite.getGlobalBounds();
}

void PushableBlock::setOnGround(bool grounded) {
    if (grounded && !onGround) {
        velocity.y = 0;
    }
    onGround = grounded;
}
