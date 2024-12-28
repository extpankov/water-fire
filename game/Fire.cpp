#include "Fire.h"
#include <iostream>
#include <stdexcept>

Fire::Fire() : currentState(AnimState::Idle), basePosition(0, 0), alive(true), facingRight(true) {
    
    
    float koef = 0.25;
    stateOffsets[AnimState::Idle][static_cast<size_t>(FirePart::Body)] = sf::Vector2f(0, 0);
    stateOffsets[AnimState::Idle][static_cast<size_t>(FirePart::HeadBack)] = sf::Vector2f(-50 * koef, -170 * koef);
    stateOffsets[AnimState::Idle][static_cast<size_t>(FirePart::Face)] = sf::Vector2f(0, -110 * koef);
    stateOffsets[AnimState::Idle][static_cast<size_t>(FirePart::Hair)] = sf::Vector2f(-55 * koef, -300 * koef);

    
    stateOffsets[AnimState::Running][static_cast<size_t>(FirePart::Body)] = sf::Vector2f(0, 0);
    stateOffsets[AnimState::Running][static_cast<size_t>(FirePart::HeadBack)] = sf::Vector2f(-50 * koef, -170 * koef);
    stateOffsets[AnimState::Running][static_cast<size_t>(FirePart::Face)] = sf::Vector2f(0, -110 * koef);
    stateOffsets[AnimState::Running][static_cast<size_t>(FirePart::Hair)] = sf::Vector2f(-155 * koef, -170 * koef);

    
    stateOffsetsLeft[AnimState::Idle][static_cast<size_t>(FirePart::Body)] = sf::Vector2f(0, 0);
    stateOffsetsLeft[AnimState::Idle][static_cast<size_t>(FirePart::HeadBack)] = sf::Vector2f(50 * koef, -170 * koef);
    stateOffsetsLeft[AnimState::Idle][static_cast<size_t>(FirePart::Face)] = sf::Vector2f(0, -110 * koef);
    stateOffsetsLeft[AnimState::Idle][static_cast<size_t>(FirePart::Hair)] = sf::Vector2f(55 * koef, -300 * koef);

    
    stateOffsetsLeft[AnimState::Running][static_cast<size_t>(FirePart::Body)] = sf::Vector2f(0, 0);
    stateOffsetsLeft[AnimState::Running][static_cast<size_t>(FirePart::HeadBack)] = sf::Vector2f(50 * koef, -170 * koef);
    stateOffsetsLeft[AnimState::Running][static_cast<size_t>(FirePart::Face)] = sf::Vector2f(0, -110 * koef);
    stateOffsetsLeft[AnimState::Running][static_cast<size_t>(FirePart::Hair)] = sf::Vector2f(155 * koef, -170 * koef);
}

void Fire::init() {
    if (!texture.loadFromFile("data/fireboy.png")) {
        throw std::runtime_error("Failed to load texture: data/fireboy.png");
    }
    
    
    for (auto& sprite : sprites) {
        sprite.setTexture(texture);
        sprite.setScale(0.25f, 0.25f);
    }
    
    initAnimations();
    currentState = AnimState::Idle;
    updatePartOffsets();
}

void Fire::updatePartOffsets() {
    
    const auto& currentOffsets = facingRight ? stateOffsets[currentState] : stateOffsetsLeft[currentState];
    for (size_t i = 0; i < static_cast<size_t>(FirePart::Count); ++i) {
        sprites[i].setPosition(basePosition + currentOffsets[i]);
    }
}

void Fire::update(float deltaTime) {
    for (size_t i = 0; i < static_cast<size_t>(FirePart::Count); ++i) {
        animations[i].update(deltaTime);
        sprites[i].setTextureRect(animations[i].getCurrentRect());
    }
    updatePartOffsets();
}

void Fire::draw(sf::RenderWindow& window) {
    if (!alive) return;
    
    for (const auto& sprite : sprites) {
        window.draw(sprite);
    }
}

void Fire::setPosition(const sf::Vector2f& pos) {
    basePosition = pos;
    updatePartOffsets();
}

void Fire::setScale(const sf::Vector2f& scale) {
    for (auto& sprite : sprites) {
        sprite.setScale(scale);
    }
}

void Fire::setDirection(bool facingRight) {
    this->facingRight = facingRight;
    for (size_t i = 0; i < static_cast<size_t>(FirePart::Count); ++i) {
        sprites[i].setScale(facingRight ? 0.25f : -0.25f, 0.25f);
    }
    updatePartOffsets();
}

void Fire::setIdle() {
    currentState = AnimState::Idle;
    for (size_t i = 0; i < static_cast<size_t>(FirePart::Count); ++i) {
        animations[i].setCurrentAnimation("idle");
    }
    updatePartOffsets();
}

void Fire::setRunning() {
    currentState = AnimState::Running;
    for (size_t i = 0; i < static_cast<size_t>(FirePart::Count); ++i) {
        animations[i].setCurrentAnimation("run");
    }
    updatePartOffsets();
}

void Fire::setJumping() {
    currentState = AnimState::Jumping;
    for (size_t i = 0; i < static_cast<size_t>(FirePart::Count); ++i) {
        animations[i].setCurrentAnimation("jump");
    }
    updatePartOffsets();
}

void Fire::setAttacking() {
    currentState = AnimState::Attacking;
    for (size_t i = 0; i < static_cast<size_t>(FirePart::Count); ++i) {
        animations[i].setCurrentAnimation("attack");
    }
    updatePartOffsets();
}

void Fire::reset(const sf::Vector2f& startPos) {
    basePosition = startPos;
    setIdle();
    updatePartOffsets();
}

void Fire::initAnimations() {
    
    for (size_t partIndex = 0; partIndex < static_cast<size_t>(FirePart::Count); ++partIndex) {
        FirePart part = static_cast<FirePart>(partIndex);
        
        
        Animation idleAnim;
        idleAnim.looped = true;
        
        
        switch (part) {
            case FirePart::Body:
                
                idleAnim.frames.push_back({sf::IntRect(3, 70, 98, 98), 0.2f});
                break;
                
            case FirePart::HeadBack:
                
                idleAnim.frames.push_back({sf::IntRect(3, 173, 195, 195), 0.2f});
                break;
                
            case FirePart::Face:
                
                idleAnim.frames.push_back({sf::IntRect(3, 709, 108, 90), 0.2f});
                break;
                
            case FirePart::Hair:
                
                idleAnim.frames.push_back({sf::IntRect(3, 374, 204, 270), 0.2f});
                idleAnim.frames.push_back({sf::IntRect(429, 374, 204, 260), 0.2f});
                idleAnim.frames.push_back({sf::IntRect(856, 374, 204, 279), 0.2f});
                idleAnim.frames.push_back({sf::IntRect(1284, 374, 200, 279), 0.2f});
                idleAnim.frames.push_back({sf::IntRect(1704, 374, 200, 279), 0.2f});

                break;
                
            default:
                break;
        }
        
        animations[partIndex].addAnimation("idle", idleAnim);
        

        
        Animation runAnim;
        runAnim.looped = true;
        
        switch (part) {
            case FirePart::Body:
                
                runAnim.frames.push_back({sf::IntRect(5, 827, 68, 82), 0.2f});
                runAnim.frames.push_back({sf::IntRect(79, 828, 68, 81), 0.2f});
                runAnim.frames.push_back({sf::IntRect(154, 827, 41, 82), 0.2f});
                runAnim.frames.push_back({sf::IntRect(201, 827, 33, 81), 0.2f});
                runAnim.frames.push_back({sf::IntRect(240, 826, 54, 86), 0.2f});
                runAnim.frames.push_back({sf::IntRect(298, 826, 61, 88), 0.2f});
                runAnim.frames.push_back({sf::IntRect(364, 826, 40, 87), 0.2f});
                runAnim.frames.push_back({sf::IntRect(408, 826, 41, 91), 0.2f});
                runAnim.frames.push_back({sf::IntRect(453, 827, 54, 91), 0.2f});

                break;
                
            case FirePart::HeadBack:
                
                runAnim.frames.push_back({sf::IntRect(3, 173, 195, 195), 0.2f});
                break;
                
            case FirePart::Face:
                
                runAnim.frames.push_back({sf::IntRect(3, 709, 108, 90), 0.2f});
                break;
                
            case FirePart::Hair:
                
                runAnim.frames.push_back({sf::IntRect(4, 1121, 177, 180), 0.2f});
                runAnim.frames.push_back({sf::IntRect(497, 1122, 193, 177), 0.2f});
                runAnim.frames.push_back({sf::IntRect(1013, 1121, 178, 175), 0.2f});
                runAnim.frames.push_back({sf::IntRect(1541, 1122, 187, 172), 0.2f});
                runAnim.frames.push_back({sf::IntRect(2056, 1122, 181, 188), 0.2f});

                break;
                
            default:
                break;
        }
        
        animations[partIndex].addAnimation("run", runAnim);
    }
}

void Fire::kill() {
    alive = false;
}

void Fire::revive() {
    alive = true;
}
