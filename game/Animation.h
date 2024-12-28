#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>


struct AnimationFrame {
    sf::IntRect rect;      
    float duration;        
};


struct Animation {
    std::vector<AnimationFrame> frames;    
    bool looped;                           
    float totalDuration;                   
};


class AnimationManager {
public:
    AnimationManager();
    
    
    void addAnimation(const std::string& name, const Animation& animation);
    
    
    void setCurrentAnimation(const std::string& name);
    
    
    void update(float deltaTime);
    
    
    sf::IntRect getCurrentRect() const;
    
    
    void reset();

private:
    std::map<std::string, Animation> animations;     
    std::string currentAnimationName;                
    size_t currentFrame;                            
    float currentTime;                              
};
