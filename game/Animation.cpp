#include "Animation.h"

AnimationManager::AnimationManager()
    : currentFrame(0)
    , currentTime(0.0f)
{
}

void AnimationManager::addAnimation(const std::string& name, const Animation& animation) {
    animations[name] = animation;
}

void AnimationManager::setCurrentAnimation(const std::string& name) {
    if (currentAnimationName != name) {
        currentAnimationName = name;
        reset();
    }
}

void AnimationManager::update(float deltaTime) {
    if (animations.empty() || currentAnimationName.empty()) {
        return;
    }

    const Animation& currentAnim = animations[currentAnimationName];
    currentTime += deltaTime;

    
    while (currentTime >= currentAnim.frames[currentFrame].duration) {
        currentTime -= currentAnim.frames[currentFrame].duration;
        currentFrame++;

        if (currentFrame >= currentAnim.frames.size()) {
            if (currentAnim.looped) {
                currentFrame = 0;
            } else {
                currentFrame = currentAnim.frames.size() - 1;
                break;
            }
        }
    }
}

sf::IntRect AnimationManager::getCurrentRect() const {
    if (animations.empty() || currentAnimationName.empty()) {
        return sf::IntRect();
    }
    return animations.at(currentAnimationName).frames[currentFrame].rect;
}

void AnimationManager::reset() {
    currentFrame = 0;
    currentTime = 0.0f;
}
