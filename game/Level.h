#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Boundary.h"
#include "Fire.h"
#include "Water.h"
#include "Door.h"
#include "Diamond.h"
#include "Hint.h"
#include "GameConstants.h"
#include "GameEngine.h"
#include "MenuState.h"
#include "Lever.h"
#include "MovingPlatform.h"
#include "PressureButton.h"
#include "ButtonPlatform.h"
#include "PushableBlock.h"

class Level {
public:
    Level(GameEngine& game);
    ~Level();
    void init(const std::string& mapFile);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window, sf::Vector2f viewPos, bool debug = false);
    void handleInput(const sf::Event& event);
    void updateFireCharacter(float deltaTime);
    void updateWaterCharacter(float deltaTime);
    void killCharacter();
    void checkDiamonds();
    void updateLevers(float deltaTime);  
    void drawLevers(sf::RenderWindow& window);  
    void resetLevel();
    void drawDeathEffects(sf::RenderWindow& window);

    bool isFinished() const { return isLevelFinished; }
    bool shouldReturnToMenu() const { return returnToMenu; }
    bool isFading() const { return isFadingOut; }
    float getFadeProgress() const { return fadeOutProgress; }
    bool isPauseMenuActive() const { return isPaused; }
    bool isSettingsMenuActive() const { return settingsMenuActive; }
    bool isDeathAnimationPlaying() const { return isDeathAnimationActive; }
    float getDeathAnimationProgress() const { return deathAnimationProgress; }

    static constexpr float FADE_SPEED = 2.0f;
    static constexpr float SETTINGS_ANIMATION_SPEED = 4.0f;

private:
    void loadDeathEffects();
    void updatePauseButtonPosition();
    void loadBoundaryFromFile(const std::string& filename);
    void updateDeathEffects(float deltaTime);
    void updateFadeOut(float deltaTime);
    void updateFinishAnimation(float deltaTime);
    bool isPointInSprite(const sf::Vector2f& point, const sf::Sprite& sprite) const;
    bool canCharacterInteractWithPlatform(const Bound_box& boundary, bool isWaterCharacter) const;
    bool canCharacterInteractWithPlatform(const sf::FloatRect& characterBounds, bool isWaterCharacter);
    bool isDeadlyPlatform(const Bound_box& boundary, bool isWaterCharacter) const;

    GameEngine& game;
    std::string currentMapFile;
    sf::RenderTexture texture;
    sf::RectangleShape drawableObject;
    sf::Vector2f size;
    std::vector<Bound_box> boundaries;
    std::vector<Bound_polygon> polygons;
    std::vector<Diamond> diamonds;
    std::vector<Hint> hints;
    std::vector<Door> doors;
    std::vector<Lever> levers;
    std::vector<std::pair<MovingPlatform, size_t>> leverPlatforms;
    std::vector<PressureButton> buttons;
    std::vector<std::pair<ButtonPlatform, std::vector<size_t>>> buttonPlatforms;
    std::vector<PushableBlock> blocks;
    sf::Vector2f start[2];
    Fire fireCharacter;
    Water waterCharacter;
    bool isLevelFinished;
    bool isDeathAnimationActive;
    bool isFadingOut;
    bool returnToMenu;
    float pauseMenuPosition;
    float deathAnimationProgress;
    float fadeOutProgress;
    float levelFinishMenuPosition;
    bool onGround;
    float verticalSpeed;
    float lastGroundY;
    bool isMovingLeft;
    bool isMovingRight;
    bool isJumping;
    bool waterOnGround;
    float waterVerticalSpeed;
    float waterLastGroundY;
    bool isWaterMovingLeft;
    bool isWaterMovingRight;
    bool isWaterJumping;
    float moveSpeed;
    float gravity;
    float jumpSpeed;
    float pixels_per_meter;
    float viewZoomFactor;
    bool gameOverSoundPlayed;
    bool isPaused;
    bool settingsMenuActive;
    float settingsMenuPosition;
    int finishAnimationStep;
    float finishAnimationTimer;
    bool allDiamondsCollected;
    bool timeUnder30Seconds;
    sf::Texture pauseButtonTexture;
    sf::Sprite pauseButton;
    sf::Texture menuButtonTexture;
    sf::Sprite menuButtonSprite;
    sf::Texture retryButtonTexture;
    sf::Sprite retryButtonSprite;
    sf::Texture resumeButtonTexture;
    sf::Sprite resumeButton;
    sf::Texture settingsButtonTexture;
    sf::Sprite settingsButton;
    sf::Texture musicOnTexture;
    sf::Texture musicOffTexture;
    sf::Sprite musicButton;
    sf::Texture soundOnTexture;
    sf::Texture soundOffTexture;
    sf::Sprite soundButton;
    sf::Texture okButtonTexture;
    sf::Sprite okButton;
    sf::SoundBuffer jumpFireBuffer;
    sf::Sound jumpFireSound;
    sf::SoundBuffer jumpWaterBuffer;
    sf::Sound jumpWaterSound;
    sf::SoundBuffer diamondBuffer;
    sf::Sound diamondSound;
    sf::SoundBuffer deathBuffer;
    sf::Sound deathSound;
    sf::SoundBuffer levelMusicBuffer;
    sf::Sound levelMusic;
    sf::SoundBuffer levelMusicOverBuffer;
    sf::Sound levelMusicOver;
    sf::SoundBuffer gameOverBuffer;
    sf::Sound gameOverSound;
    sf::SoundBuffer buttonClickBuffer;
    sf::Sound buttonClickSound;
    sf::SoundBuffer waterStepsBuffer;
    sf::SoundBuffer fireStepsBuffer;
    sf::SoundBuffer doorBuffer;
    sf::SoundBuffer levelFinishBuffer;
    sf::Sound waterStepsSound;
    sf::Sound fireStepsSound;
    sf::Sound doorSound;
    sf::Sound levelFinishSound;
    sf::RectangleShape darkOverlay;
    sf::Texture levelFinishBackgroundTexture;
    sf::Sprite levelFinishBackground;
    sf::Clock levelTimer;
    sf::Time finishTime;
    sf::Font timerFont;
    sf::Text timerText;
    sf::Texture settingsMenuTexture;
    sf::Sprite settingsMenuSprite;
    sf::Texture pauseMenuBackgroundTexture;
    sf::Sprite pauseMenuBackground;
    sf::Texture pairTexture;
    sf::Texture diamondsTexture;
    sf::Texture timeTexture;
    sf::Texture okIconTexture;
    sf::Texture xmarkIconTexture;
    sf::Texture arrowTexture;
    sf::Texture diamondEmptyTexture;
    sf::Texture diamondFullTexture;
    sf::Sprite pairSprite;
    sf::Sprite diamondsSprite;
    sf::Sprite timeSprite;
    sf::Sprite okIcon1Sprite;
    sf::Sprite okIcon2Sprite;
    sf::Sprite okIcon3Sprite;
    sf::Sprite xmarkIcon1Sprite;
    sf::Sprite xmarkIcon2Sprite;
    sf::Sprite xmarkIcon3Sprite;
    sf::Sprite arrowSprite;
    sf::Sprite diamondEmptySprite;
    sf::Sprite diamondFullSprite;
    std::vector<sf::Sprite> diamondEmptySprites;
    std::vector<sf::Sprite> diamondFullSprites;
    sf::Texture gameOverTexture;
    sf::Sprite gameOverSprite;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Sprite finalDiamondSprite;
    sf::Texture continueButtonTexture;
    sf::Sprite continueButtonSprite;
};
