#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "AnimState.h"
#include <array>
#include <map>


enum class FirePart {
    Body,       
    HeadBack,   
    Face,       
    Hair,       
    Count       
};

class Fire {
public:
    Fire();
    void init();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    
    void setIdle();
    void setRunning();
    void setJumping();
    void setAttacking();
    void setDirection(bool facingRight);
    
    
    void setPosition(const sf::Vector2f& pos);
    void setScale(const sf::Vector2f& scale);
    
    
    sf::Vector2f getPosition() const { return basePosition; }
    sf::FloatRect getGlobalBounds() const { 
        return sf::FloatRect(basePosition.x - 20, basePosition.y - 20, 40, 40); 
    }
    
    
    void reset(const sf::Vector2f& startPos);
    
    
    void kill();
    
    
    void revive();
    
    
    bool isAlive() const { return alive; }

    
    void moveLeft();
    void moveRight();
    void stopMoving();
    void jump();
    
private:
    void initAnimations();
    void updatePartOffsets();
    
    sf::Texture texture;
    std::array<sf::Sprite, static_cast<size_t>(FirePart::Count)> sprites;
    std::array<AnimationManager, static_cast<size_t>(FirePart::Count)> animations;
    
    AnimState currentState;
    sf::Vector2f basePosition;
    bool alive;
    bool facingRight;  
    
    
    std::map<AnimState, std::array<sf::Vector2f, static_cast<size_t>(FirePart::Count)>> stateOffsets;
    std::map<AnimState, std::array<sf::Vector2f, static_cast<size_t>(FirePart::Count)>> stateOffsetsLeft;
    
    float verticalSpeed;
    bool isMovingLeft;
    bool isMovingRight;
    bool isJumping;
    bool onGround;
    
    static constexpr float MOVEMENT_SPEED = 200.0f;
    static constexpr float JUMP_SPEED = -400.0f;
    static constexpr float GRAVITY = 981.0f;
};
