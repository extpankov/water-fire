#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>


enum class DoorType {
    Water,
    Fire,
    Universal
};


enum class DoorState {
    Closed,
    Opening,
    Open,
    Closing
};

class Door {
private:
    sf::Sprite doorBackground;
    sf::Sprite doorFrame;
    sf::Sprite doorBase;
    sf::Sprite doorIcon;
    
    DoorType doorType;
    DoorState state;
    float animationProgress;
    sf::Vector2f position;
    
    sf::IntRect backgroundRect;
    sf::IntRect frameRect;
    sf::IntRect baseRect;
    sf::IntRect iconRect;
    
    float scale;
    bool isDragging;
    sf::Sprite* selectedSprite;
    sf::IntRect* selectedRect;
    
    
    sf::SoundBuffer doorSoundBuffer;
    sf::Sound doorSound;
    
    static sf::Texture spriteSheet;
    
    void updateSpritePositions();

public:
    Door(DoorType type);
    void init();
    void loadTextures();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void open();
    void close();
    bool isOpened() const { return state == DoorState::Open; }
    bool isAnimating() const { return state == DoorState::Opening || state == DoorState::Closing; }
    
    void handleEvent(const sf::Event& event);
    void setPosition(const sf::Vector2f& pos);
    void setScale(float newScale);
    sf::Vector2f getPosition() const { return position; }
    sf::FloatRect getGlobalBounds() const;
    DoorType getType() const { return doorType; }
    bool canInteractWithCharacter(bool isWaterCharacter) const;
    bool tryInteract(bool isWaterCharacter);
};
