#include "Door.h"
#include <iostream>
#include <unistd.h>
#include <SFML/Audio.hpp>


sf::Texture Door::spriteSheet;


const float ANIMATION_SPEED = 1.5f;  

Door::Door(DoorType type) : scale(1.0f), isDragging(false), selectedSprite(nullptr), selectedRect(nullptr),
                           state(DoorState::Closed), animationProgress(0.0f), doorType(type) {
    
    if (!doorSoundBuffer.loadFromFile("data/soundtracks/Door.ogg")) {
        std::cerr << "Error: Failed to load door sound!" << std::endl;
    }
    doorSound.setBuffer(doorSoundBuffer);
    doorSound.setVolume(100.0f);  
    
    
    if (doorType == DoorType::Fire) {
        
        backgroundRect = sf::IntRect(3, 277, 47, 55);
        frameRect = sf::IntRect(50, 103, 57, 62);
        baseRect = sf::IntRect(4, 103, 40, 53);
        iconRect = sf::IntRect(113, 103, 15, 17);
    } else {
        
        backgroundRect = sf::IntRect(3, 277, 47, 55);  
        frameRect = sf::IntRect(50, 190, 57, 62);      
        baseRect = sf::IntRect(4, 190, 40, 53);        
        iconRect = sf::IntRect(113, 189, 12, 21);      
    }
    
    loadTextures();
    
}

void Door::init() {
    
    updateSpritePositions();
}

void Door::setPosition(const sf::Vector2f& pos) {
    position = pos;
    updateSpritePositions();
}

void Door::loadTextures() {
    std::cout << "\n=== Loading door textures ===\n";
    
    
    std::cout << "Initial texture rectangles:\n";
    std::cout << "Background: (" << backgroundRect.left << ", " << backgroundRect.top << ", " 
              << backgroundRect.width << ", " << backgroundRect.height << ")\n";
    std::cout << "Frame: (" << frameRect.left << ", " << frameRect.top << ", " 
              << frameRect.width << ", " << frameRect.height << ")\n";
    std::cout << "Base: (" << baseRect.left << ", " << baseRect.top << ", " 
              << baseRect.width << ", " << baseRect.height << ")\n";
    std::cout << "Icon: (" << iconRect.left << ", " << iconRect.top << ", " 
              << iconRect.width << ", " << iconRect.height << ")\n\n";
    
    
    std::string texturePath = "data/door.png"; 
    if (!spriteSheet.loadFromFile(texturePath)) {
        std::cerr << "Error: Failed to load texture from '" << texturePath << "'\n";
        
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cerr << "Current working directory: " << cwd << "\n";
        }
        std::cerr << "Please ensure the texture file exists at the specified path\n";
        return;
    }
    
    
    sf::Vector2u texSize = spriteSheet.getSize();
    std::cout << "Loaded texture size: " << texSize.x << "x" << texSize.y << "\n";
    std::cout << "Texture address after loading: " << &spriteSheet << "\n";
    
    
    auto validateRect = [&](const sf::IntRect& rect, const std::string& name) {
        bool valid = true;
        if (rect.left < 0 || rect.top < 0) {
            std::cerr << "Error: " << name << " position is negative\n";
            valid = false;
        }
        if (rect.left + rect.width > static_cast<int>(texSize.x)) {
            std::cerr << "Error: " << name << " extends beyond texture width\n";
            valid = false;
        }
        if (rect.top + rect.height > static_cast<int>(texSize.y)) {
            std::cerr << "Error: " << name << " extends beyond texture height\n";
            valid = false;
        }
        return valid;
    };
    
    bool allValid = true;
    allValid &= validateRect(backgroundRect, "Background");
    allValid &= validateRect(frameRect, "Frame");
    allValid &= validateRect(baseRect, "Base");
    allValid &= validateRect(iconRect, "Icon");
    
    if (!allValid) {
        std::cerr << "Error: Some texture rectangles are invalid!\n";
        return;
    }
    
    
    doorFrame.setTexture(spriteSheet);
    doorBackground.setTexture(spriteSheet);
    doorBase.setTexture(spriteSheet);
    doorIcon.setTexture(spriteSheet);
    
    
    doorFrame.setTextureRect(frameRect);
    doorBackground.setTextureRect(backgroundRect);
    doorBase.setTextureRect(baseRect);
    doorIcon.setTextureRect(iconRect);
    
    
    doorBackground.setScale(scale, scale);
    doorFrame.setScale(scale, scale);
    doorBase.setScale(scale, scale);
    doorIcon.setScale(scale, scale);
    
    
    doorFrame.setOrigin(0, 0);
    doorBackground.setOrigin(0, 0);
    doorBase.setOrigin(0, 0);
    doorIcon.setOrigin(0, 0);
    
    
    std::cout << "\nFinal sprite settings:\n";
    auto printSprite = [](const sf::Sprite& sprite, const std::string& name) {
        const sf::IntRect& rect = sprite.getTextureRect();
        const sf::Vector2f& scale = sprite.getScale();
        const sf::Vector2f& origin = sprite.getOrigin();
        std::cout << name << ":\n"
                 << "  TextureRect: (" << rect.left << ", " << rect.top << ", " 
                 << rect.width << ", " << rect.height << ")\n"
                 << "  Scale: " << scale.x << ", " << scale.y << "\n"
                 << "  Origin: " << origin.x << ", " << origin.y << "\n";
    };
    
    printSprite(doorFrame, "Frame");
    printSprite(doorBackground, "Background");
    printSprite(doorBase, "Base");
    printSprite(doorIcon, "Icon");
    
    std::cout << "=== Door textures loaded successfully ===\n\n";
}

void Door::updateSpritePositions() {
    
    float frameOffsetX = 0;  
    float frameOffsetY = 0;
    
    float bgOffsetX = (frameRect.width - backgroundRect.width) / 2.0f;
    float bgOffsetY = (frameRect.height - backgroundRect.height) / 2.0f;
    
    float baseOffsetX = (frameRect.width - baseRect.width) / 2.0f;
    float baseOffsetY = (frameRect.height - baseRect.height) / 2.0f;
    
    float iconOffsetX = (frameRect.width - iconRect.width) / 2.0f;
    float iconOffsetY = (frameRect.height - iconRect.height) / 2.0f;
    
    
    doorFrame.setPosition(position.x + frameOffsetX, position.y + frameOffsetY);
    doorBackground.setPosition(position.x + bgOffsetX, position.y + bgOffsetY);
    doorBase.setPosition(position.x + baseOffsetX, position.y + baseOffsetY);
    doorIcon.setPosition(position.x + iconOffsetX, position.y + iconOffsetY);
}

void Door::update(float deltaTime) {
    
    if (!doorBackground.getTexture() || !doorFrame.getTexture() || 
        !doorBase.getTexture() || !doorIcon.getTexture()) {
        std::cerr << "Error: Sprites lost texture during update! Reloading...\n";
        loadTextures();
        return;
    }

    if (state == DoorState::Opening) {
        animationProgress += deltaTime / ANIMATION_SPEED;
        if (animationProgress >= 1.0f) {
            animationProgress = 1.0f;
            state = DoorState::Open;
        }
    } else if (state == DoorState::Closing) {
        animationProgress -= deltaTime / ANIMATION_SPEED;
        if (animationProgress <= 0.0f) {
            animationProgress = 0.0f;
            state = DoorState::Closed;
        }
    }

    
    float frameOffsetX = 0;  
    float frameOffsetY = 0;
    
    float bgOffsetX = (frameRect.width - backgroundRect.width) / 2.0f;
    float bgOffsetY = (frameRect.height - backgroundRect.height) / 2.0f;
    
    float baseOffsetX = (frameRect.width - baseRect.width) / 2.0f;
    float baseOffsetY = (frameRect.height - baseRect.height) / 2.0f;
    
    float iconOffsetX = (frameRect.width - iconRect.width) / 2.0f;
    float iconOffsetY = (frameRect.height - iconRect.height) / 2.0f;
    
    doorFrame.setPosition(position.x + frameOffsetX, position.y + frameOffsetY);
    doorBackground.setPosition(position.x + bgOffsetX, position.y + bgOffsetY);
    doorBase.setPosition(position.x + baseOffsetX, position.y + baseOffsetY);
    doorIcon.setPosition(position.x + iconOffsetX, position.y + iconOffsetY);
    
    
    int remainingHeight = static_cast<int>(baseRect.height * (1.0f - animationProgress));
    
    if (remainingHeight > 0) {
        
        int cutFromTop = baseRect.height - remainingHeight;
        
        
        doorBase.setTextureRect(sf::IntRect(
            baseRect.left,
            baseRect.top + cutFromTop,
            baseRect.width,
            remainingHeight
        ));
        
        
        int iconBottom = iconRect.top + iconRect.height - baseRect.top;  
        if (cutFromTop < iconBottom) {
            int iconCutFromTop = std::max(0, cutFromTop - (iconRect.top - baseRect.top));
            int iconRemainingHeight = iconRect.height - iconCutFromTop;
            
            if (iconRemainingHeight > 0) {
                doorIcon.setTextureRect(sf::IntRect(
                    iconRect.left,
                    iconRect.top + iconCutFromTop,
                    iconRect.width,
                    iconRemainingHeight
                ));
            }
        }
    }

    
    static bool firstUpdate = true;
    if (firstUpdate) {
        std::cout << "\nSprite state after first update:\n";
        std::cout << "Background texture rect: " << doorBackground.getTextureRect().left << ", "
                 << doorBackground.getTextureRect().top << ", "
                 << doorBackground.getTextureRect().width << ", "
                 << doorBackground.getTextureRect().height << "\n";
        std::cout << "Frame texture rect: " << doorFrame.getTextureRect().left << ", "
                 << doorFrame.getTextureRect().top << ", "
                 << doorFrame.getTextureRect().width << ", "
                 << doorFrame.getTextureRect().height << "\n";
        firstUpdate = false;
    }
}

void Door::draw(sf::RenderWindow& window) {
    
    static bool firstDraw = true;
    if (firstDraw) {
        std::cout << "\nSprite validation during first draw:\n";
        auto validateSprite = [](const sf::Sprite& sprite, const std::string& name) {
            const sf::Color& color = sprite.getColor();
            const sf::Texture* tex = sprite.getTexture();
            std::cout << name << ":\n"
                     << "  Color: " << (int)color.r << ", " << (int)color.g << ", " 
                     << (int)color.b << ", " << (int)color.a << "\n"
                     << "  Has texture: " << (tex != nullptr ? "yes" : "no") << "\n"
                     << "  Texture address: " << tex << "\n"
                     << "  TextureRect: " << sprite.getTextureRect().left << ", "
                     << sprite.getTextureRect().top << ", "
                     << sprite.getTextureRect().width << ", "
                     << sprite.getTextureRect().height << "\n";
        };
        
        validateSprite(doorBackground, "Background");
        validateSprite(doorFrame, "Frame");
        validateSprite(doorBase, "Base");
        validateSprite(doorIcon, "Icon");
        firstDraw = false;
    }

    
    if (!doorBackground.getTexture() || !doorFrame.getTexture() || 
        !doorBase.getTexture() || !doorIcon.getTexture()) {
        std::cerr << "Error: Some sprites lost their texture! Reloading...\n";
        loadTextures();
        return;
    }

    
    sf::Color spriteColor(255, 255, 255, 255);  
    doorBackground.setColor(spriteColor);
    doorFrame.setColor(spriteColor);
    doorBase.setColor(spriteColor);
    doorIcon.setColor(spriteColor);

    window.draw(doorBackground);
    window.draw(doorFrame);
    
    
    if (animationProgress < 1.0f) {
        window.draw(doorBase);
        
        
        int cutFromTop = static_cast<int>(baseRect.height * animationProgress);
        int iconBottom = iconRect.top + iconRect.height - baseRect.top;
        if (cutFromTop < iconBottom) {
            window.draw(doorIcon);
        }
    }
}

bool Door::canInteractWithCharacter(bool isWaterCharacter) const {
    switch (doorType) {
        case DoorType::Water:
            return isWaterCharacter;
        case DoorType::Fire:
            return !isWaterCharacter;
        case DoorType::Universal:
            return true;
        default:
            return false;
    }
}

bool Door::tryInteract(bool isWaterCharacter) {
    if (canInteractWithCharacter(isWaterCharacter)) {
        
        if (state == DoorState::Closed || state == DoorState::Closing) {
            open();
            return true;
        }
        return true; 
    }
    return false;
}

void Door::open() {
    if (state == DoorState::Closed || state == DoorState::Closing) {
        state = DoorState::Opening;
        doorSound.play();
    }
}

void Door::close() {
    
    if ((state == DoorState::Open || state == DoorState::Opening) && 
        (animationProgress > 0.0f)) {
        state = DoorState::Closing;
        doorSound.play();
    }
}

sf::FloatRect Door::getGlobalBounds() const {
    
    return doorFrame.getGlobalBounds();
}

void Door::handleEvent(const sf::Event& event) {
    
    if (!isDragging) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            
            
            if (doorBackground.getGlobalBounds().contains(mousePos)) {
                selectedSprite = &doorBackground;
                selectedRect = &backgroundRect;
                isDragging = true;
            } else if (doorFrame.getGlobalBounds().contains(mousePos)) {
                selectedSprite = &doorFrame;
                selectedRect = &frameRect;
                isDragging = true;
            } else if (doorBase.getGlobalBounds().contains(mousePos)) {
                selectedSprite = &doorBase;
                selectedRect = &baseRect;
                isDragging = true;
            } else if (doorIcon.getGlobalBounds().contains(mousePos)) {
                selectedSprite = &doorIcon;
                selectedRect = &iconRect;
                isDragging = true;
            }
        }
    } else {
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            isDragging = false;
            selectedSprite = nullptr;
            selectedRect = nullptr;
        } else if (event.type == sf::Event::MouseMoved) {
            if (selectedSprite && selectedRect) {
                sf::Vector2f newPos(event.mouseMove.x, event.mouseMove.y);
                selectedSprite->setPosition(newPos);
                
                
                sf::Vector2f localPos = selectedSprite->getPosition();
                selectedRect->left = static_cast<int>(localPos.x);
                selectedRect->top = static_cast<int>(localPos.y);
            }
        }
    }
}
