#include "Hint.h"
#include <cmath>
#include <iostream>
#include <sstream>

sf::Font Hint::font;
const float Hint::SHOW_RADIUS = 200.0f;  
const float Hint::HIDE_RADIUS = 200.0f;  

Hint::Hint(const sf::Vector2f& pos, const std::string& text)
    : position(pos), waterVisited(false), fireVisited(false) {
    textLines = splitText(text);
}

void Hint::loadFont() {
    if (!font.loadFromFile("data/arial.ttf")) {
        std::cerr << "Error loading hint font!" << std::endl;
    }
}

void Hint::draw(sf::RenderWindow& window, const sf::Vector2f& viewPos, const sf::Vector2f& waterPos, const sf::Vector2f& firePos) {
    
    if (shouldHide(waterPos, firePos)) return;
    
    float lineHeight = 30.0f;
    float totalHeight = lineHeight * textLines.size();
    
    for (size_t i = 0; i < textLines.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(textLines[i]);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color(228, 206, 83, 255));  
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2.0f);
        
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2.0f, 0);
        
        
        float yOffset = -totalHeight / 2.0f + i * lineHeight;
        text.setPosition(position.x + viewPos.x, position.y + viewPos.y + yOffset);
        
        window.draw(text);
    }
}

bool Hint::isInRange(const sf::Vector2f& characterPos) const {
    return getDistance(characterPos) <= SHOW_RADIUS;
}

bool Hint::shouldHide(const sf::Vector2f& waterPos, const sf::Vector2f& firePos) const {
    
    return getDistance(waterPos) > SHOW_RADIUS && getDistance(firePos) > SHOW_RADIUS;
}

float Hint::getDistance(const sf::Vector2f& point) const {
    float dx = position.x - point.x;
    float dy = position.y - point.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<std::string> Hint::splitText(const std::string& text) const {
    std::vector<std::string> lines;
    std::string currentText = text;
    
    
    if (!currentText.empty() && currentText[0] == '"') {
        currentText = currentText.substr(1);
    }
    if (!currentText.empty() && currentText[currentText.length()-1] == '"') {
        currentText = currentText.substr(0, currentText.length()-1);
    }
    
    size_t pos = 0;
    while ((pos = currentText.find("\\n")) != std::string::npos) {
        lines.push_back(currentText.substr(0, pos));
        currentText = currentText.substr(pos + 2);
    }
    if (!currentText.empty()) {
        lines.push_back(currentText);
    }
    
    return lines;
}
