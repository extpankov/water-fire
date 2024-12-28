#pragma once

#include <SFML/Graphics.hpp>

enum class DiamondType {
    Water,
    Fire
};

class Diamond {
public:
    Diamond(DiamondType type, const sf::Vector2f& position);
    
    void draw(sf::RenderWindow& window, const sf::Vector2f& viewPos);
    bool canCollect(bool isWaterCharacter) const;
    sf::FloatRect getGlobalBounds() const;
    bool isCollected() const { return collected; }
    void collect();
    
    static void loadTexture();
    
private:
    DiamondType type;
    sf::Sprite sprite;
    bool collected;
    
    static sf::Texture diamondTexture;
    
    
    static const sf::IntRect waterDiamondRect;
    static const sf::IntRect fireDiamondRect;
};
