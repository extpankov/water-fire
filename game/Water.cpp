#include "Water.h"
#include <iostream>
#include <stdexcept>

Water::Water() : currentState(AnimState::Idle), basePosition(0, 0), alive(true), facingRight(true) {
    
    
    float koef = 0.0625;
    stateOffsets[AnimState::Idle][static_cast<size_t>(WaterPart::Body)] = sf::Vector2f(0, 0);
    stateOffsets[AnimState::Idle][static_cast<size_t>(WaterPart::HeadBack)] = sf::Vector2f(-200 * koef, -700 * koef);
    stateOffsets[AnimState::Idle][static_cast<size_t>(WaterPart::Face)] = sf::Vector2f(-190 * koef, -650 * koef);
    stateOffsets[AnimState::Idle][static_cast<size_t>(WaterPart::Hair)] = sf::Vector2f(-90 * koef, -1400 * koef);

    
    stateOffsets[AnimState::Running][static_cast<size_t>(WaterPart::Body)] = sf::Vector2f(0, 0);
    stateOffsets[AnimState::Running][static_cast<size_t>(WaterPart::HeadBack)] = sf::Vector2f(-200 * koef, -700 * koef);
    stateOffsets[AnimState::Running][static_cast<size_t>(WaterPart::Face)] = sf::Vector2f(-200 * koef, -650 * koef);
    stateOffsets[AnimState::Running][static_cast<size_t>(WaterPart::Hair)] = sf::Vector2f(-800 * koef, -750 * koef);

    
    stateOffsetsLeft[AnimState::Idle][static_cast<size_t>(WaterPart::Body)] = sf::Vector2f(0, 0);
    stateOffsetsLeft[AnimState::Idle][static_cast<size_t>(WaterPart::HeadBack)] = sf::Vector2f(200 * koef, -700 * koef);
    stateOffsetsLeft[AnimState::Idle][static_cast<size_t>(WaterPart::Face)] = sf::Vector2f(190 * koef, -650 * koef);
    stateOffsetsLeft[AnimState::Idle][static_cast<size_t>(WaterPart::Hair)] = sf::Vector2f(90 * koef, -1400 * koef);

    
    stateOffsetsLeft[AnimState::Running][static_cast<size_t>(WaterPart::Body)] = sf::Vector2f(0, 0);
    stateOffsetsLeft[AnimState::Running][static_cast<size_t>(WaterPart::HeadBack)] = sf::Vector2f(200 * koef, -700 * koef);
    stateOffsetsLeft[AnimState::Running][static_cast<size_t>(WaterPart::Face)] = sf::Vector2f(200 * koef, -650 * koef);
    stateOffsetsLeft[AnimState::Running][static_cast<size_t>(WaterPart::Hair)] = sf::Vector2f(800 * koef, -750 * koef);
}

void Water::init() {
    if (!texture.loadFromFile("data/watergirl.png")) {
        throw std::runtime_error("Failed to load texture: data/watergirl.png");
    }
    
    
    for (auto& sprite : sprites) {
        sprite.setTexture(texture);
        sprite.setScale(0.25f, 0.25f); 
    }
    
    initAnimations();
    currentState = AnimState::Idle;
    updatePartOffsets();
}

void Water::updatePartOffsets() {
    
    const auto& currentOffsets = facingRight ? stateOffsets[currentState] : stateOffsetsLeft[currentState];
    for (size_t i = 0; i < static_cast<size_t>(WaterPart::Count); ++i) {
        sprites[i].setPosition(basePosition + currentOffsets[i]);
    }
}

void Water::update(float deltaTime) {
    for (size_t i = 0; i < static_cast<size_t>(WaterPart::Count); ++i) {
        animations[i].update(deltaTime);
        sprites[i].setTextureRect(animations[i].getCurrentRect());
    }
    updatePartOffsets();
}

void Water::draw(sf::RenderWindow& window) {
    if (!alive) return;
    
    for (const auto& sprite : sprites) {
        window.draw(sprite);
    }
}

void Water::setPosition(const sf::Vector2f& pos) {
    basePosition = pos;
    updatePartOffsets();
}

void Water::setScale(const sf::Vector2f& scale) {
    for (auto& sprite : sprites) {
        sprite.setScale(scale);
    }
}

void Water::setDirection(bool facingRight) {
    this->facingRight = facingRight;
    for (size_t i = 0; i < static_cast<size_t>(WaterPart::Count); ++i) {
        sprites[i].setScale(facingRight ? 0.25f : -0.25f, 0.25f);
    }
    updatePartOffsets();
}

void Water::setIdle() {
    currentState = AnimState::Idle;
    for (size_t i = 0; i < static_cast<size_t>(WaterPart::Count); ++i) {
        animations[i].setCurrentAnimation("idle");
    }
    updatePartOffsets();
}

void Water::setRunning() {
    currentState = AnimState::Running;
    for (size_t i = 0; i < static_cast<size_t>(WaterPart::Count); ++i) {
        animations[i].setCurrentAnimation("run");
    }
    updatePartOffsets();
}

void Water::setJumping() {
    currentState = AnimState::Jumping;
    for (size_t i = 0; i < static_cast<size_t>(WaterPart::Count); ++i) {
        animations[i].setCurrentAnimation("jump");
    }
    updatePartOffsets();
}

void Water::setAttacking() {
    currentState = AnimState::Attacking;
    for (size_t i = 0; i < static_cast<size_t>(WaterPart::Count); ++i) {
        animations[i].setCurrentAnimation("attack");
    }
    updatePartOffsets();
}

void Water::reset(const sf::Vector2f& startPos) {
    basePosition = startPos;
    currentState = AnimState::Idle;
    alive = true;
    for (size_t i = 0; i < static_cast<size_t>(WaterPart::Count); ++i) {
        animations[i].setCurrentAnimation("idle");
    }
    updatePartOffsets();
}

void Water::kill() {
    alive = false;
}

void Water::revive() {
    alive = true;
}

void Water::initAnimations() {
    
    for (size_t partIndex = 0; partIndex < static_cast<size_t>(WaterPart::Count); ++partIndex) {
        WaterPart part = static_cast<WaterPart>(partIndex);
        
        
        Animation idleAnim;
        idleAnim.looped = true;
        
        switch (part) {
            case WaterPart::Body:
                
                idleAnim.frames.push_back({sf::IntRect(3, 294, 99, 99), 0.2f});
                break;
                
            case WaterPart::HeadBack:
                
                idleAnim.frames.push_back({sf::IntRect(4, 72, 195, 196), 0.2f});
                break;
                
            case WaterPart::Face:
                
                idleAnim.frames.push_back({sf::IntRect(4, 398, 188, 190), 0.2f});
                break;
                
            case WaterPart::Hair:
                
                idleAnim.frames.push_back({sf::IntRect(5, 593, 151, 243), 0.2f});
                idleAnim.frames.push_back({sf::IntRect(468, 593, 163, 266), 0.2f});
                idleAnim.frames.push_back({sf::IntRect(748, 593, 177, 262), 0.2f});
                idleAnim.frames.push_back({sf::IntRect(1032, 592, 183, 250), 0.2f});
                idleAnim.frames.push_back({sf::IntRect(1293, 592, 163, 244), 0.2f});
                idleAnim.frames.push_back({sf::IntRect(1565, 592, 162, 269), 0.2f});
                break;
                
            default:
                break;
        }
        
        animations[partIndex].addAnimation("idle", idleAnim);

        
        Animation runAnim;
        runAnim.looped = true;
        
        switch (part) {
            case WaterPart::Body:
                
                runAnim.frames.push_back({sf::IntRect(81, 886, 67, 83), 0.2f});
                runAnim.frames.push_back({sf::IntRect(152, 885, 45, 84), 0.2f});
                runAnim.frames.push_back({sf::IntRect(202, 886, 32, 82), 0.2f});
                runAnim.frames.push_back({sf::IntRect(239, 887, 55, 84), 0.2f});
                runAnim.frames.push_back({sf::IntRect(299, 886, 57, 85), 0.2f});
                runAnim.frames.push_back({sf::IntRect(362, 886, 42, 84), 0.2f});
                runAnim.frames.push_back({sf::IntRect(407, 886, 40, 88), 0.2f});
                runAnim.frames.push_back({sf::IntRect(407, 886, 40, 88), 0.2f});
                break;
                
            case WaterPart::HeadBack:
                
                runAnim.frames.push_back({sf::IntRect(4, 72, 195, 196), 0.2f});
                break;
                
            case WaterPart::Face:
                
                runAnim.frames.push_back({sf::IntRect(3, 980, 194, 191), 0.2f});
                break;
                
            case WaterPart::Hair:
                
                runAnim.frames.push_back({sf::IntRect(5, 1177, 344, 140), 0.2f});
                runAnim.frames.push_back({sf::IntRect(352, 1175, 344, 141), 0.2f});
                runAnim.frames.push_back({sf::IntRect(701, 1176, 356, 138), 0.2f});
                runAnim.frames.push_back({sf::IntRect(1062, 1176, 369, 139), 0.2f});
                runAnim.frames.push_back({sf::IntRect(1435, 1176, 374, 132), 0.2f});
                runAnim.frames.push_back({sf::IntRect(1814, 1176, 352, 136), 0.2f});
                break;
                
            default:
                break;
        }
        
        animations[partIndex].addAnimation("run", runAnim);
    }
}
