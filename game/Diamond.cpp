#include "Diamond.h"
#include <iostream>

sf::Texture Diamond::diamondTexture;
const sf::IntRect Diamond::waterDiamondRect(3, 4, 22, 18);    
const sf::IntRect Diamond::fireDiamondRect(28, 4, 22, 18);    

Diamond::Diamond(DiamondType type, const sf::Vector2f& position) 
    : type(type), collected(false) {
    sprite.setTexture(diamondTexture);
    
    
    if (type == DiamondType::Water) {
        sprite.setTextureRect(waterDiamondRect);
    } else {
        sprite.setTextureRect(fireDiamondRect);
    }
    
    
    sprite.setPosition(position);
}

void Diamond::loadTexture() {
    if (!diamondTexture.loadFromFile("data/diamonds.png")) {
        std::cerr << "Error loading diamond texture!" << std::endl;
    }
}

void Diamond::draw(sf::RenderWindow& window, const sf::Vector2f& viewPos) {
    if (!collected) {
        sf::Vector2f originalPos = sprite.getPosition();
        sprite.setPosition(originalPos + viewPos);
        window.draw(sprite);
        sprite.setPosition(originalPos);  
    }
}

bool Diamond::canCollect(bool isWaterCharacter) const {
    
    
    return !collected && 
           ((isWaterCharacter && type == DiamondType::Water) ||
            (!isWaterCharacter && type == DiamondType::Fire));
}

void Diamond::collect() {
    collected = true;
}

sf::FloatRect Diamond::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}
