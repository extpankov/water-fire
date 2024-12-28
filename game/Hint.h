#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Hint {
public:
    Hint(const sf::Vector2f& position, const std::string& text);
    
    void draw(sf::RenderWindow& window, const sf::Vector2f& viewPos, const sf::Vector2f& waterPos, const sf::Vector2f& firePos);
    bool isInRange(const sf::Vector2f& characterPos) const;
    bool wasVisited(bool isWaterCharacter) const { return isWaterCharacter ? waterVisited : fireVisited; }
    void setVisited(bool isWaterCharacter) { if (isWaterCharacter) waterVisited = true; else fireVisited = true; }
    bool shouldShow() const { return true; }  
    bool shouldHide(const sf::Vector2f& waterPos, const sf::Vector2f& firePos) const;
    
    static void loadFont();
    
private:
    sf::Vector2f position;
    std::vector<std::string> textLines;
    bool waterVisited;
    bool fireVisited;
    
    static const float SHOW_RADIUS;
    static const float HIDE_RADIUS;
    static sf::Font font;
    
    float getDistance(const sf::Vector2f& point) const;
    std::vector<std::string> splitText(const std::string& text) const;
};
