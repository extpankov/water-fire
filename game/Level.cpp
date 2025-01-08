#include "Level.h"
#include "GameEngine.h"
#include "MenuState.h"
#include "PlayState.h"
#include "GameConstants.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iomanip>

Level::Level(GameEngine& game) 
    : game(game), isMovingLeft(false), isMovingRight(false),
      isJumping(false), isWaterMovingLeft(false), isWaterMovingRight(false),
      isWaterJumping(false), moveSpeed(300.0f), verticalSpeed(0.0f),
      waterVerticalSpeed(0.0f), onGround(false), waterOnGround(false),
      lastGroundY(0.0f), waterLastGroundY(0.0f), gravity(981.0f),
      jumpSpeed(-450.0f), pixels_per_meter(30.0f), viewZoomFactor(1.0f),
      isDeathAnimationActive(false), deathAnimationProgress(0.0f),
      isFadingOut(false), fadeOutProgress(0.0f), returnToMenu(false),
      gameOverSoundPlayed(false), isPaused(false), pauseMenuPosition(0.0f),
      settingsMenuActive(false), settingsMenuPosition(0.0f),
      isLevelFinished(false), levelFinishMenuPosition(0.0f),
      finishAnimationStep(0), finishAnimationTimer(0.0f),
      allDiamondsCollected(false), timeUnder30Seconds(false),
      finishTime(sf::Time::Zero)  
{
    texture.create(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT);
    drawableObject.setSize(sf::Vector2f(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT));
    
    if (!timerFont.loadFromFile("assets/fonts/arial.ttf")) {
        if (!timerFont.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
            
            timerFont.loadFromFile("/System/Library/Fonts/Helvetica.ttc");
        }
    }
    
    timerText.setFont(timerFont);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color(228, 206, 83, 255));
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setOutlineThickness(2.0f);
    
    levelTimer.restart();
    
    
    fireCharacter = Fire();
    waterCharacter = Water();
    
    loadDeathEffects();
    
    
    if (!pauseButtonTexture.loadFromFile("data/pause_pic.png")) {
        std::cerr << "Ошибка при загрузке текстуры кнопки паузы" << std::endl;
    }
    
    if (!resumeButtonTexture.loadFromFile("data/resume.png")) {
        std::cerr << "Ошибка при загрузке текстуры кнопки resume" << std::endl;
    }
    
    if (!menuButtonTexture.loadFromFile("data/menu.png")) {
        std::cerr << "Ошибка при загрузке текстуры кнопки menu" << std::endl;
    }
    
    if (!retryButtonTexture.loadFromFile("data/retry.png")) {
        std::cerr << "Ошибка при загрузке текстуры кнопки retry" << std::endl;
    }
    
    if (!settingsButtonTexture.loadFromFile("data/settings.png")) {
        std::cerr << "Ошибка при загрузке текстур1ы кнопки настроек" << std::endl;
    }
    
    if (!settingsMenuTexture.loadFromFile("data/settings_menu.png")) {
        std::cerr << "Ошибка при загрузке текстуры фона меню настроек" << std::endl;
    }
    settingsMenuSprite.setTexture(settingsMenuTexture);
    
    
    sf::FloatRect settingsMenuBounds = settingsMenuSprite.getLocalBounds();
    settingsMenuSprite.setOrigin(settingsMenuBounds.width/2, settingsMenuBounds.height/2);
    
    if (!musicOnTexture.loadFromFile("data/settings_music_button_on.png")) {
        std::cerr << "Ошибка при загрузке текстуры музыки вкл" << std::endl;
    }
    
    if (!musicOffTexture.loadFromFile("data/settings_music_button_off.png")) {
        std::cerr << "Ошибка при загрузке текстуры музыки выкл" << std::endl;
    }
    
    if (!soundOnTexture.loadFromFile("data/settings_sound_button_on.png")) {
        std::cerr << "Ошибка при загрузке текстуры звука вкл" << std::endl;
    }
    
    if (!soundOffTexture.loadFromFile("data/settings_sound_button_off.png")) {
        std::cerr << "Ошибка при загрузке текстуры звука выкл" << std::endl;
    }
    
    if (!okButtonTexture.loadFromFile("data/settings_ok_button.png")) {
        std::cerr << "Ошибка при загрузке текстуры кнопки ок" << std::endl;
    }
    
    if (!pauseMenuBackgroundTexture.loadFromFile("data/settings_menu.png")) {
        std::cerr << "Ошибка при загрузке текстуры фона меню паузы" << std::endl;
    }
    
    pauseButton.setTexture(pauseButtonTexture);
    resumeButton.setTexture(resumeButtonTexture);
    menuButtonSprite.setTexture(menuButtonTexture);
    retryButtonSprite.setTexture(retryButtonTexture);
    pauseMenuBackground.setTexture(pauseMenuBackgroundTexture);
    
    
    settingsButton.setTexture(settingsButtonTexture);
    musicButton.setTexture(game.is_music_enabled() ? musicOnTexture : musicOffTexture);
    soundButton.setTexture(game.is_sound_enabled() ? soundOnTexture : soundOffTexture);
    okButton.setTexture(okButtonTexture);
    
    
    sf::FloatRect menuBounds = menuButtonSprite.getLocalBounds();
    sf::FloatRect retryBounds = retryButtonSprite.getLocalBounds();
    sf::FloatRect resumeBounds = resumeButton.getLocalBounds();
    sf::FloatRect settingsBounds = settingsButton.getLocalBounds();
    sf::FloatRect musicBounds = musicButton.getLocalBounds();
    sf::FloatRect soundBounds = soundButton.getLocalBounds();
    sf::FloatRect okBounds = okButton.getGlobalBounds();
    
    menuButtonSprite.setOrigin(menuBounds.width/2, menuBounds.height/2);
    retryButtonSprite.setOrigin(retryBounds.width/2, retryBounds.height/2);
    resumeButton.setOrigin(resumeBounds.width/2, resumeBounds.height/2);
    settingsButton.setOrigin(settingsBounds.width/2, settingsBounds.height/2);
    musicButton.setOrigin(musicBounds.width/2, musicBounds.height/2);
    soundButton.setOrigin(soundBounds.width/2, soundBounds.height/2);
    okButton.setOrigin(okBounds.width/2, okBounds.height/2);
    
    
    float settingsScale = 1.0f;
    settingsButton.setScale(settingsScale, settingsScale);
    
    
    sf::FloatRect backgroundRect = pauseMenuBackground.getLocalBounds();
    pauseMenuBackground.setOrigin(backgroundRect.width/2, backgroundRect.height/2);
    
    
    updatePauseButtonPosition();
    
    darkOverlay.setSize(sf::Vector2f(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT));
    darkOverlay.setPosition(0, 0);
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 0));
    
    if (!jumpFireBuffer.loadFromFile("data/soundtracks/Jump fb.ogg")) {
        std::cerr << "Ошибка при загрузке звука прыжка огня" << std::endl;
    }
    if (!jumpWaterBuffer.loadFromFile("data/soundtracks/Jump wg.ogg")) {
        std::cerr << "Ошибка при загрузке звука прыжка воды" << std::endl;
    }
    if (!waterStepsBuffer.loadFromFile("data/soundtracks/Water Steps.ogg")) {
        std::cerr << "Ошибка при загрузке звука шагов воды" << std::endl;
    }
    if (!fireStepsBuffer.loadFromFile("data/soundtracks/Fire Steps.ogg")) {
        std::cerr << "Ошибка при загрузке звука шагов огня" << std::endl;
    }
    if (!doorBuffer.loadFromFile("data/soundtracks/Door.ogg")) {
        std::cerr << "Ошибка при загрузке звука двери" << std::endl;
    }
    if (!diamondBuffer.loadFromFile("data/soundtracks/Diamond.ogg")) {
        std::cerr << "Ошибка при загрузке звука алмаза" << std::endl;
    }
    if (!deathBuffer.loadFromFile("data/soundtracks/Death.ogg")) {
        std::cerr << "Ошибка при загрузке звука смерти" << std::endl;
    }
    if (!levelMusicBuffer.loadFromFile("data/soundtracks/Level Music.ogg")) {
        std::cerr << "Ошибка при загрузке музыки уровня" << std::endl;
    }
    if (!levelMusicOverBuffer.loadFromFile("data/soundtracks/Level Music Over.ogg")) {
        std::cerr << "Ошибка при загрузке музыки окончания уровня" << std::endl;
    }
    if (!gameOverBuffer.loadFromFile("data/soundtracks/Level Music Over.ogg")) {
        std::cerr << "Ошибка при загрузке звука Game Over" << std::endl;
    }
    if (!levelFinishBuffer.loadFromFile("data/soundtracks/Level Finish.ogg")) {
        std::cerr << "Ошибка при загрузке звука завершения уровня" << std::endl;
    }
    
    jumpFireSound.setBuffer(jumpFireBuffer);
    jumpWaterSound.setBuffer(jumpWaterBuffer);
    waterStepsSound.setBuffer(waterStepsBuffer);
    fireStepsSound.setBuffer(fireStepsBuffer);
    doorSound.setBuffer(doorBuffer);
    diamondSound.setBuffer(diamondBuffer);
    deathSound.setBuffer(deathBuffer);
    levelMusic.setBuffer(levelMusicBuffer);
    levelMusicOver.setBuffer(levelMusicOverBuffer);
    gameOverSound.setBuffer(gameOverBuffer);
    levelFinishSound.setBuffer(levelFinishBuffer);
    
    levelMusic.setLoop(true);
    levelMusic.setVolume(50.f);  
    if (game.is_music_enabled()) {
        levelMusic.play();
    }
    
    if (!levelFinishBackgroundTexture.loadFromFile("data/settings_menu.png")) {
        std::cerr << "Ошибка при загрузке фона завершения уровня" << std::endl;
    }
    levelFinishBackground.setTexture(levelFinishBackgroundTexture);
    
    
    sf::FloatRect finishBounds = levelFinishBackground.getLocalBounds();
    levelFinishBackground.setOrigin(finishBounds.width/2, finishBounds.height/2);
    
    
    if (!pairTexture.loadFromFile("data/finish/pair.png")) {
        std::cerr << "Ошибка при загрузке текстуры pair.png" << std::endl;
    }
    if (!diamondsTexture.loadFromFile("data/finish/diamonds.png")) {
        std::cerr << "Ошибка при загрузке текстуры diamonds.png" << std::endl;
    }
    if (!timeTexture.loadFromFile("data/finish/time.png")) {
        std::cerr << "Ошибка при загрузке текстуры time.png" << std::endl;
    }
    if (!okIconTexture.loadFromFile("data/finish/ok_icon.png")) {
        std::cerr << "Ошибка при загрузке текстуры ok_icon.png" << std::endl;
    }
    if (!xmarkIconTexture.loadFromFile("data/finish/xmark_icon.png")) {
        std::cerr << "Ошибка при загрузке текстуры xmark_icon.png" << std::endl;
    }
    if (!arrowTexture.loadFromFile("data/finish/arrow.png")) {
        std::cerr << "Ошибка при загрузке текстуры arrow.png" << std::endl;
    }
    if (!diamondEmptyTexture.loadFromFile("data/finish/diamond_empty.png")) {
        std::cerr << "Ошибка при загрузке текстуры diamond_empty.png" << std::endl;
    }
    if (!diamondFullTexture.loadFromFile("data/finish/diamond_full.png")) {
        std::cerr << "Ошибка при загрузке текстуры diamond_full.png" << std::endl;
    }
    if (!continueButtonTexture.loadFromFile("data/continue.png")) {
        std::cerr << "Ошибка при загрузке текстуры continue.png" << std::endl;
    }
    
    pairSprite.setTexture(pairTexture);
    diamondsSprite.setTexture(diamondsTexture);
    timeSprite.setTexture(timeTexture);
    okIcon1Sprite.setTexture(okIconTexture);
    okIcon2Sprite.setTexture(okIconTexture);
    okIcon3Sprite.setTexture(okIconTexture);
    arrowSprite.setTexture(arrowTexture);
    diamondEmptySprite.setTexture(diamondEmptyTexture);
    diamondFullSprite.setTexture(diamondFullTexture);
    continueButtonSprite.setTexture(continueButtonTexture);
    continueButtonSprite.setScale(0.5f, 0.5f);
    
    auto centerOrigin = [](sf::Sprite& sprite) {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    };
    
    centerOrigin(pairSprite);
    centerOrigin(diamondsSprite);
    centerOrigin(timeSprite);
    centerOrigin(okIcon1Sprite);
    centerOrigin(okIcon2Sprite);
    centerOrigin(okIcon3Sprite);
    centerOrigin(arrowSprite);
    centerOrigin(diamondEmptySprite);
    centerOrigin(diamondFullSprite);
    centerOrigin(continueButtonSprite);
}

Level::~Level() {
    
    boundaries.clear();
    polygons.clear();
    doors.clear();
    diamonds.clear();
    hints.clear();
    blocks.clear();
}

void Level::loadDeathEffects() {
    if (!backgroundTexture.loadFromFile("data/settings_menu.png")) {
        std::cerr << "Failed to load texture: data/settings_menu.png" << std::endl;
        return;
    }
    
    sf::RenderTexture renderTexture;
    renderTexture.create(backgroundTexture.getSize().x, backgroundTexture.getSize().y);
    renderTexture.clear(sf::Color::Transparent);
    
    sf::Sprite sprite;
    sprite.setTexture(backgroundTexture);
    renderTexture.draw(sprite);
    renderTexture.display();
    
    drawableObject.setTexture(&renderTexture.getTexture(), true);
    
    if (!gameOverTexture.loadFromFile("data/game_over.png")) {
        std::cerr << "Ошибка при загрузке game over: data/game_over.png" << std::endl;
    } else {
        gameOverSprite.setTexture(gameOverTexture);
        sf::FloatRect rect = gameOverSprite.getLocalBounds();
        gameOverSprite.setOrigin(rect.width/2, rect.height/2);
        gameOverSprite.setPosition(
            GameConstants::WINDOW_WIDTH/2,
            GameConstants::WINDOW_HEIGHT * 1.5f  
        );
    }
    
    if (!menuButtonTexture.loadFromFile("data/menu.png")) {
        std::cerr << "Ошибка при загрузке кнопки меню: data/menu.png" << std::endl;
    } else {
        menuButtonSprite.setTexture(menuButtonTexture);
        sf::FloatRect rect = menuButtonSprite.getLocalBounds();
        menuButtonSprite.setOrigin(rect.width/2, rect.height/2);
        menuButtonSprite.setPosition(
            GameConstants::WINDOW_WIDTH/2 - 100,
            GameConstants::WINDOW_HEIGHT * 1.5f  
        );
    }

    if (!retryButtonTexture.loadFromFile("data/retry.png")) {
        std::cerr << "Ошибка при загрузке кнопки retry: data/retry.png" << std::endl;
    } else {
        retryButtonSprite.setTexture(retryButtonTexture);
        sf::FloatRect rect = retryButtonSprite.getLocalBounds();
        retryButtonSprite.setOrigin(rect.width/2, rect.height/2);
        retryButtonSprite.setPosition(
            GameConstants::WINDOW_WIDTH/2 + 100,
            GameConstants::WINDOW_HEIGHT * 1.5f  
        );
    }

    darkOverlay.setSize(sf::Vector2f(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT));
    darkOverlay.setPosition(0, 0);
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 0));
}

void Level::updateDeathEffects(float deltaTime) {
    if (isDeathAnimationActive) {
        deathAnimationProgress = std::min(1.0f, deathAnimationProgress + deltaTime);
        
        
        sf::Color overlayColor = sf::Color(0, 0, 0, 128);
        overlayColor.a = static_cast<sf::Uint8>(deathAnimationProgress * 128); 
        darkOverlay.setFillColor(overlayColor);

        
        float startY = GameConstants::WINDOW_HEIGHT * 1.5f;
        float centerX = GameConstants::WINDOW_WIDTH / 2.0f;

        
        float backgroundProgress = std::max(0.0f, (deathAnimationProgress - 0.1f) / 0.9f);
        float backgroundEndY = GameConstants::WINDOW_HEIGHT / 2.0f;
        float backgroundCurrentY = startY + (backgroundEndY - startY) * backgroundProgress;
        settingsMenuSprite.setPosition(centerX, backgroundCurrentY);

        
        float gameOverProgress = std::max(0.0f, (deathAnimationProgress - 0.2f) / 0.8f);
        float gameOverEndY = GameConstants::WINDOW_HEIGHT / 2.0f - 100.0f;
        float gameOverCurrentY = startY + (gameOverEndY - startY) * gameOverProgress;
        gameOverSprite.setPosition(centerX, gameOverCurrentY);

        
        float buttonsProgress = std::max(0.0f, (deathAnimationProgress - 0.4f) / 0.6f);
        float buttonsEndY = GameConstants::WINDOW_HEIGHT / 2.0f + 50.0f;
        float buttonsCurrentY = startY + (buttonsEndY - startY) * buttonsProgress;
        
        float menuX = centerX - 100.0f;
        float retryX = centerX + 100.0f;
        menuButtonSprite.setPosition(menuX, buttonsCurrentY);
        retryButtonSprite.setPosition(retryX, buttonsCurrentY);

        
        viewZoomFactor = 1.0f + deathAnimationProgress * 0.2f;

        
        if (deathAnimationProgress >= 1.0f && !gameOverSoundPlayed) {
            levelMusic.stop();
            gameOverSound.play();
            gameOverSoundPlayed = true;
        }
    }
}

void Level::drawDeathEffects(sf::RenderWindow& window) {
    if (isDeathAnimationActive) {
        
        window.draw(darkOverlay);
        window.draw(settingsMenuSprite);  
        window.draw(gameOverSprite);
        window.draw(menuButtonSprite);
        window.draw(retryButtonSprite);
    }
}

void Level::resetLevel() {
    
    levelMusic.stop();
    gameOverSound.stop();
    
    
    isDeathAnimationActive = false;
    deathAnimationProgress = 0.0f;
    isFadingOut = false;
    fadeOutProgress = 0.0f;
    gameOverSoundPlayed = false;
    isLevelFinished = false;
    isPaused = false;
    pauseMenuPosition = 0.0f;
    settingsMenuActive = false;
    settingsMenuPosition = 0.0f;
    
    
    onGround = true;  
    verticalSpeed = 0.0f;
    lastGroundY = 0.0f;
    isMovingLeft = false;
    isMovingRight = false;
    isJumping = false;
    
    waterOnGround = true;  
    waterVerticalSpeed = 0.0f;
    waterLastGroundY = 0.0f;
    isWaterMovingLeft = false;
    isWaterMovingRight = false;
    isWaterJumping = false;
    
    
    fireCharacter.init();
    waterCharacter.init();
    fireCharacter.setIdle();
    waterCharacter.setIdle();
    
    
    boundaries.clear();
    polygons.clear();
    diamonds.clear();
    hints.clear();
    doors.clear();
    levers.clear();
    leverPlatforms.clear();
    buttons.clear();
    buttonPlatforms.clear();
    blocks.clear();
    
    
    init(currentMapFile);
    
    
    levelTimer.restart();
    if (game.is_music_enabled()) {
        levelMusic.play();
    }
}

void Level::init(const std::string& mapFile) {
    currentMapFile = mapFile;
    std::cout << "Initializing level with map: " << mapFile << std::endl;
    
    std::fstream bf;
    bf.open(mapFile, std::ios::in);
    if (!bf.is_open()) {
        std::cerr << "Failed to open map file!" << std::endl;
        return;
    }

    std::string textureFile;
    std::getline(bf, textureFile);  
    std::cout << "Texture path: " << textureFile << std::endl;

    sf::Texture tempTexture;
    if (!tempTexture.loadFromFile(textureFile)) {
        std::cerr << "Failed to load texture: " << textureFile << std::endl;
        return;
    }
    
    texture.create(tempTexture.getSize().x, tempTexture.getSize().y);
    texture.clear(sf::Color::Transparent);
    
    sf::Sprite sprite;
    sprite.setTexture(tempTexture);
    texture.draw(sprite);
    texture.display();
    
    drawableObject.setTexture(&texture.getTexture(), true);
    
    auto texSize = drawableObject.getTexture()->getSize();
    drawableObject.setSize(static_cast<sf::Vector2f>(texSize));
    size.x = static_cast<float>(texSize.x) / pixels_per_meter;
    size.y = static_cast<float>(texSize.y) / pixels_per_meter;

    boundaries.clear();
    polygons.clear();
    doors.clear();
    diamonds.clear();
    hints.clear();
    blocks.clear();

    std::string line;
    while (std::getline(bf, line)) {
        std::cout << "Processing line: " << line << std::endl;
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "fire_door" || type == "water_door") {
            float x, y;
            iss >> x >> y;
            
            std::cout << "Creating " << type << " at position (" << x << ", " << y << ")" << std::endl;
            
            
            Door door(type == "fire_door" ? DoorType::Fire : DoorType::Water);
            door.init();
            door.setPosition(sf::Vector2f(x, y));
            doors.push_back(std::move(door));
            std::cout << "Door added to level, total doors: " << doors.size() << std::endl;
            continue;
        }

        if (type == "lever") {
            float x, y;
            if (!(iss >> x >> y)) {
                std::cerr << "Failed to parse coordinates for type: " << type << std::endl;
                std::cerr << "Processing line: " << line << std::endl;
                continue;
            }
            std::cout << "Creating lever at position: " << x << ", " << y << std::endl;
            Lever lever;
            lever.init();  
            lever.setPosition(sf::Vector2f(x, y));
            levers.push_back(lever);
            continue;
        }

        if (type == "lever_platform") {
            float x1, y1, x2, y2;
            size_t leverIndex;
            if (!(iss >> x1 >> y1 >> x2 >> y2 >> leverIndex)) {
                std::cerr << "Failed to parse coordinates for type: " << type << std::endl;
                std::cerr << "Processing line: " << line << std::endl;
                continue;
            }
            std::cout << "Creating lever platform at position: " << x1 << ", " << y1 << std::endl;
            MovingPlatform platform(
                sf::Vector2f(x1, y1),
                sf::Vector2f(x2, y2)
            );
            platform.init();
            if (leverIndex < levers.size()) {
                leverPlatforms.push_back(std::make_pair(platform, leverIndex));
            }
            continue;
        }

        if (type == "button") {
            float x, y;
            if (!(iss >> x >> y)) {
                std::cerr << "Failed to parse coordinates for type: " << type << std::endl;
                std::cerr << "Processing line: " << line << std::endl;
                continue;
            }
            std::cout << "Creating button at position: " << x << ", " << y << std::endl;
            PressureButton button;
            button.init();
            button.setPosition(sf::Vector2f(x, y));
            buttons.push_back(button);
            continue;
        }

        if (type == "button_platform") {
            float x1, y1, x2, y2;
            if (!(iss >> x1 >> y1 >> x2 >> y2)) {
                std::cerr << "Failed to parse coordinates for type: " << type << std::endl;
                std::cerr << "Processing line: " << line << std::endl;
                continue;
            }
            
            
            std::vector<size_t> buttonIndices;
            size_t buttonIndex;
            while (iss >> buttonIndex) {
                buttonIndices.push_back(buttonIndex);
            }
            
            std::cout << "Creating button platform from " << x1 << "," << y1 << " to " << x2 << "," << y2 << std::endl;
            ButtonPlatform platform(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2));
            platform.init();
            buttonPlatforms.emplace_back(std::make_pair(platform, buttonIndices));
            continue;
        }

        if (type == "diamond") {
            float x = 0, y = 0;
            std::string diamondType;
            
            
            if (!(iss >> x >> y >> diamondType)) {
                std::cerr << "Failed to parse diamond line. Format should be: diamond x y type" << std::endl;
                continue;
            }
            
            diamondType.erase(0, diamondType.find_first_not_of(" \t\r\n"));
            diamondType.erase(diamondType.find_last_not_of(" \t\r\n") + 1);
            
            DiamondType dt = (diamondType == "water") ? DiamondType::Water : DiamondType::Fire;

            
            sf::Vector2f pos(x, y);
            
            diamonds.emplace_back(dt, pos);
            std::cout << "Successfully created " << diamondType << " diamond at position (" << x << ", " << y << ")" << std::endl;
            continue;
        } 

        if (type == "hint") {
            float x, y;
            std::string hintText;
            
            if (!(iss >> x >> y)) {
                std::cerr << "Failed to parse hint coordinates" << std::endl;
                continue;
            }
            
            
            std::getline(iss, hintText);
            
            hintText.erase(0, hintText.find_first_not_of(" \t"));
            
            hints.emplace_back(sf::Vector2f(x, y), hintText);
            std::cout << "Created hint at position (" << x << ", " << y << ") with text: " << hintText << std::endl;
            continue;
        }

        if (type == "block") {
            float x, y;
            std::cout << "Processing block line: " << line << std::endl;
            if (!(iss >> x >> y)) {
                std::cerr << "Failed to parse coordinates for type: block" << std::endl;
                std::cerr << "Stream state: " << iss.rdstate() << std::endl;
                std::string remaining;
                iss >> remaining;
                std::cerr << "Remaining content: " << remaining << std::endl;
                continue;
            }
            std::cout << "Successfully parsed block coordinates: x=" << x << ", y=" << y << std::endl;
            blocks.emplace_back();
            blocks.back().init();
            blocks.back().setPosition(sf::Vector2f(x, y));  
            continue;
        }

        if (type == "water_start" || type == "fire_start") {
            float x, y, a, b;
            std::string p;
            if (!(iss >> x >> y >> a >> b >> p)) {
                std::cerr << "Failed to parse coordinates for type: " << type << std::endl;
                continue;
            }
            
            x /= pixels_per_meter;
            y /= pixels_per_meter;
            a /= pixels_per_meter;
            b /= pixels_per_meter;

            if (type == "water_start") {
                start[0] = { x, a };
            } else {
                start[1] = { x, a };
            }
            continue;
        }

        if (type == "poly") {
            float x, y, a, b;
            std::string p;
            if (!(iss >> x >> y >> a >> b >> p)) {
                std::cerr << "Failed to parse coordinates for poly" << std::endl;
                continue;
            }
            
            std::cout << "\nLoading polygon..." << std::endl;
            std::vector<sf::Vector2f> points;
            
            
            points.emplace_back(sf::Vector2f(x / pixels_per_meter, y / pixels_per_meter));
            points.emplace_back(sf::Vector2f(a / pixels_per_meter, b / pixels_per_meter));
            std::cout << "Point 1: " << x << ", " << y << " (in meters: " << x/pixels_per_meter << ", " << y/pixels_per_meter << ")" << std::endl;
            std::cout << "Point 2: " << a << ", " << b << " (in meters: " << a/pixels_per_meter << ", " << b/pixels_per_meter << ")" << std::endl;
            
            
            std::string line;
            std::getline(bf, line);
            
            char* str = const_cast<char*>(line.c_str());
            char* token = strtok(str, " ");
            float coords[2];
            int coord_idx = 0;
            
            while (token != nullptr) {
                if (strcmp(token, "p") == 0) {
                    break;
                }
                
                float value = std::stof(token);
                coords[coord_idx] = value;
                coord_idx++;
                
                if (coord_idx == 2) {
                    std::cout << "Point: " << coords[0] << ", " << coords[1] 
                              << " (in meters: " << coords[0]/pixels_per_meter << ", " << coords[1]/pixels_per_meter << ")" << std::endl;
                    points.emplace_back(sf::Vector2f(coords[0] / pixels_per_meter, coords[1] / pixels_per_meter));
                    coord_idx = 0;
                }
                
                token = strtok(nullptr, " ");
            }
            
            std::cout << "Final polygon points in meters:" << std::endl;
            for (const auto& p : points) {
                std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
            }
            
            if (!points.empty()) {
                polygons.emplace_back(points, Bound_polygon::Bound_type::SOLID);
            }
            continue;
        }
        
        float x, y, a, b;
        std::string p;
        if (!(iss >> x >> y >> a >> b >> p)) {
            std::cerr << "Failed to parse coordinates for type: " << type << std::endl;
            continue;
        }
        
        x /= pixels_per_meter;
        y /= pixels_per_meter;
        a /= pixels_per_meter;
        b /= pixels_per_meter;

        Bound_box::Bound_type bt;
        if (type == "wall") bt = Bound_box::Bound_type::SOLID;
        else if (type == "rib") bt = Bound_box::Bound_type::bl_rib;
        else if (type == "red") bt = Bound_box::Bound_type::bl_red;
        else if (type == "blue") bt = Bound_box::Bound_type::bl_blue;
        else if (type == "green") bt = Bound_box::Bound_type::bl_green;
        else if (type == "r_door") bt = Bound_box::Bound_type::bl_r_door;
        else if (type == "b_door") bt = Bound_box::Bound_type::bl_b_door;
        else if (type == "water_platform") bt = Bound_box::Bound_type::WATER_PLATFORM;
        else if (type == "lava_platform") bt = Bound_box::Bound_type::LAVA_PLATFORM;
        else if (type == "poison_platform") bt = Bound_box::Bound_type::POISON_PLATFORM;
        else {
            std::cerr << "Unknown type: " << type << std::endl;
            continue;
        }

        boundaries.emplace_back(sf::Vector2f(x, y), sf::Vector2f(a, b), bt);
    }
    bf.close();

    
    if (mapFile.find("map_new_2") != std::string::npos) {
        
        if (!levelMusicBuffer.loadFromFile("data/soundtracks/speed level.ogg")) {
            std::cerr << "Failed to load level music" << std::endl;
        }
        if (!levelMusicOverBuffer.loadFromFile("data/soundtracks/speed finish.mp3")) {
            std::cerr << "Failed to load finish music" << std::endl;
        }
    } else {
        
        if (!levelMusicBuffer.loadFromFile("data/soundtracks/Level Music.ogg")) {
            std::cerr << "Failed to load level music" << std::endl;
        }
        if (!levelMusicOverBuffer.loadFromFile("data/soundtracks/Level Music Over.ogg")) {
            std::cerr << "Failed to load finish music" << std::endl;
        }
    }
    
    levelMusic.setBuffer(levelMusicBuffer);
    levelMusicOver.setBuffer(levelMusicOverBuffer);
    levelMusic.setLoop(true);
    levelMusic.setVolume(50.f);  
    if (game.is_music_enabled()) {
        levelMusic.play();
    }
    
    fireCharacter.init();
    waterCharacter.init();
    fireCharacter.setIdle();
    waterCharacter.setIdle();
    
    fireCharacter.setPosition(sf::Vector2f(start[1].x * pixels_per_meter, start[1].y * pixels_per_meter));
    waterCharacter.setPosition(sf::Vector2f(start[0].x * pixels_per_meter, start[0].y * pixels_per_meter));
    
    fireCharacter.revive();
    waterCharacter.revive();

    fireCharacter.setScale(sf::Vector2f(0.25f, 0.25f));
    waterCharacter.setScale(sf::Vector2f(0.25f, 0.25f));

    Diamond::loadTexture();
    if (!diamondBuffer.loadFromFile("data/soundtracks/Diamond.ogg")) {
        std::cerr << "Error loading diamond sound!" << std::endl;
    }
    diamondSound.setBuffer(diamondBuffer);
    diamondSound.setVolume(100.0f); 
    Hint::loadFont();
}

void Level::loadBoundaryFromFile(const std::string& filename) {
    std::ifstream bf(filename);
    if (!bf.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string textureFile;  
    std::getline(bf, textureFile);  
    std::cout << "Texture path: " << textureFile << std::endl;

    sf::Texture tempTexture;
    if (!tempTexture.loadFromFile(textureFile)) {
        std::cerr << "Failed to load texture: " << textureFile << std::endl;
        return;
    }

    sf::RenderTexture renderTexture;
    renderTexture.create(tempTexture.getSize().x, tempTexture.getSize().y);
    renderTexture.clear(sf::Color::Transparent);
    
    sf::Sprite sprite;
    sprite.setTexture(tempTexture);
    renderTexture.draw(sprite);
    renderTexture.display();
    
    drawableObject.setTexture(&renderTexture.getTexture(), true);
    
    auto texSize = drawableObject.getTexture()->getSize();
    drawableObject.setSize(static_cast<sf::Vector2f>(texSize));
    size.x = static_cast<float>(texSize.x) / pixels_per_meter;
    size.y = static_cast<float>(texSize.y) / pixels_per_meter;

    boundaries.clear();
    polygons.clear();
    doors.clear();
    diamonds.clear();
    hints.clear();
    blocks.clear();

    std::string line;
    while (std::getline(bf, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        
        if (type == "lever") {
            float x, y;
            iss >> x >> y;
            Lever lever;
            lever.init();
            lever.setPosition(sf::Vector2f(x * pixels_per_meter, y * pixels_per_meter));
            levers.push_back(lever);
        }
        else if (type == "lever_platform") {
            float x1, y1, x2, y2;
            size_t leverIndex;
            iss >> x1 >> y1 >> x2 >> y2 >> leverIndex;
            
            if (leverIndex < levers.size()) {
                MovingPlatform platform(
                    sf::Vector2f(x1 * pixels_per_meter, y1 * pixels_per_meter),
                    sf::Vector2f(x2 * pixels_per_meter, y2 * pixels_per_meter)
                );
                platform.init();
                leverPlatforms.push_back(std::make_pair(platform, leverIndex));
            }
        }
        else if (type == "button") {
            float x, y;
            iss >> x >> y;
            PressureButton button;
            button.init();
            button.setPosition(sf::Vector2f(x * pixels_per_meter, y * pixels_per_meter));
            buttons.push_back(button);
        }
        else if (type == "button_platform") {
            float x1, y1, x2, y2;
            iss >> x1 >> y1 >> x2 >> y2;
            
            
            std::vector<size_t> buttonIndices;
            size_t buttonIndex;
            while (iss >> buttonIndex) {
                buttonIndices.push_back(buttonIndex);
            }
            
            ButtonPlatform platform(
                sf::Vector2f(x1 * pixels_per_meter, y1 * pixels_per_meter),
                sf::Vector2f(x2 * pixels_per_meter, y2 * pixels_per_meter)
            );
            platform.init();
            buttonPlatforms.emplace_back(std::make_pair(platform, buttonIndices));
        }
        else if (type == "boundary") {
            float x, y, width, height;
            int type;
            
            if (!(iss >> x >> y >> width >> height >> type)) {
                std::cerr << "Error reading boundary data" << std::endl;
                continue;
            }
            
            sf::Vector2f point1(x, y);
            sf::Vector2f point2(x + width, y + height);
            Bound_box boundary(point1, point2, static_cast<Bound_box::Bound_type>(type));
            boundaries.push_back(boundary);
        }
        else if (type == "block") {
            float x, y;
            std::cout << "Processing block line: " << line << std::endl;
            if (!(iss >> x >> y)) {
                std::cerr << "Failed to parse coordinates for type: block" << std::endl;
                std::cerr << "Stream state: " << iss.rdstate() << std::endl;
                std::string remaining;
                iss >> remaining;
                std::cerr << "Remaining content: " << remaining << std::endl;
                continue;
            }
            std::cout << "Successfully parsed block coordinates: x=" << x << ", y=" << y << std::endl;
            blocks.emplace_back();
            blocks.back().init();
            blocks.back().setPosition(sf::Vector2f(x, y));  
        }
    }
}

void Level::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::View currentView = game.get_window().getView();
        sf::Vector2i pixelPos = sf::Mouse::getPosition(game.get_window());
        game.get_window().setView(game.get_window().getDefaultView());
        sf::Vector2f worldPos = game.get_window().mapPixelToCoords(pixelPos);
        
        if (isDeathAnimationActive && deathAnimationProgress >= 1.0f) {
            sf::FloatRect menuBounds = menuButtonSprite.getGlobalBounds();
            sf::FloatRect retryBounds = retryButtonSprite.getGlobalBounds();
            
            if (menuBounds.contains(worldPos)) {
                isFadingOut = true;
                fadeOutProgress = 0.0f;
                returnToMenu = true;
                levelMusic.stop();
                return;
            }
            else if (retryBounds.contains(worldPos)) {
                isFadingOut = true;
                fadeOutProgress = 0.0f;
                returnToMenu = false;
                levelMusic.stop();
                return;
            }
        }
        else if (isLevelFinished) {
            sf::FloatRect continueBounds = continueButtonSprite.getGlobalBounds();
            if (continueBounds.contains(worldPos)) {
                if (currentMapFile == "data/maps/map_new_1.bnd") {
                    std::cout << "Setting max level to 1 before transitioning to level select" << std::endl;
                    game.set_max_level(1);
                }
                game.change_state(new LevelSelectState(game));
                return;
            }
        }
        else if (!isDeathAnimationActive && !isFadingOut) {
            if (isPaused) {
                if (settingsMenuActive && settingsMenuPosition >= 1.0f) {
                    if (isPointInSprite(worldPos, musicButton)) {
                        bool isMusicEnabled = game.is_music_enabled();
                        game.set_music_enabled(!isMusicEnabled);
                        musicButton.setTexture(!isMusicEnabled ? musicOnTexture : musicOffTexture);
                        if (!isMusicEnabled) {
                            levelMusic.play();
                        } else {
                            levelMusic.pause();
                        }
                    }
                    else if (isPointInSprite(worldPos, soundButton)) {
                        bool isSoundEnabled = game.is_sound_enabled();
                        game.set_sound_enabled(!isSoundEnabled);
                        soundButton.setTexture(!isSoundEnabled ? soundOnTexture : soundOffTexture);
                    }
                    else if (isPointInSprite(worldPos, okButton)) {
                        settingsMenuActive = false;
                    }
                } else if (!settingsMenuActive || settingsMenuPosition <= 0.0f) {
                    if (isPointInSprite(worldPos, resumeButton)) {
                        isPaused = false;
                        if (game.is_music_enabled()) {
                            levelMusic.play();
                        }
                    }
                    else if (isPointInSprite(worldPos, menuButtonSprite)) {
                        isFadingOut = true;
                        fadeOutProgress = 0.0f;
                        returnToMenu = true;
                        levelMusic.stop();
                    }
                    else if (isPointInSprite(worldPos, retryButtonSprite) && !isLevelFinished) {
                        isFadingOut = true;
                        fadeOutProgress = 0.0f;
                        returnToMenu = false;
                        levelMusic.stop();
                    }
                    else if (isPointInSprite(worldPos, settingsButton)) {
                        settingsMenuActive = true;
                    }
                }
            }
            else if (isPointInSprite(worldPos, pauseButton) && !isLevelFinished) {
                isPaused = true;
                levelMusic.pause();
            }
        }
        
        game.get_window().setView(currentView);
    }
    else if (!isDeathAnimationActive && !isFadingOut) {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    if (settingsMenuActive) {
                        settingsMenuActive = false;
                    } else {
                        isPaused = !isPaused;
                        if (isPaused) {
                            levelMusic.pause();
                        } else {
                            if (game.is_music_enabled()) {
                                levelMusic.play();
                            }
                        }
                    }
                    break;
                case sf::Keyboard::A:
                    isWaterMovingLeft = true;
                    break;
                case sf::Keyboard::D:
                    isWaterMovingRight = true;
                    break;
                case sf::Keyboard::W:
                    if (waterOnGround) {
                        isWaterJumping = true;
                        waterVerticalSpeed = jumpSpeed;
                        waterOnGround = false;
                        if (game.is_sound_enabled()) {
                            jumpWaterSound.play();
                        }
                    }
                    break;
                case sf::Keyboard::Left:
                    isMovingLeft = true;
                    break;
                case sf::Keyboard::Right:
                    isMovingRight = true;
                    break;
                case sf::Keyboard::Up:
                    if (onGround) {
                        isJumping = true;
                        verticalSpeed = jumpSpeed;
                        onGround = false;
                        if (game.is_sound_enabled()) {
                            jumpFireSound.play();
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            switch (event.key.code) {
                case sf::Keyboard::A:
                    isWaterMovingLeft = false;
                    break;
                case sf::Keyboard::D:
                    isWaterMovingRight = false;
                    break;
                case sf::Keyboard::W:
                    isWaterJumping = false;
                    break;
                case sf::Keyboard::Left:
                    isMovingLeft = false;
                    break;
                case sf::Keyboard::Right:
                    isMovingRight = false;
                    break;
                case sf::Keyboard::Up:
                    isJumping = false;
                    break;
                default:
                    break;
            }
        }
    }
}

void Level::update(float deltaTime) {
    if (isPaused) {
        float targetPosition = 1.0f;
        if (pauseMenuPosition < targetPosition) {
            pauseMenuPosition = std::min(pauseMenuPosition + 4.0f * deltaTime, targetPosition);
        }
        
        if (settingsMenuActive) {
            if (settingsMenuPosition < 1.0f) {
                settingsMenuPosition = std::min(settingsMenuPosition + SETTINGS_ANIMATION_SPEED * deltaTime, 1.0f);
            }
        } else {
            if (settingsMenuPosition > 0.0f) {
                settingsMenuPosition = std::max(settingsMenuPosition - SETTINGS_ANIMATION_SPEED * deltaTime, 0.0f);
            }
        }
        
        updatePauseButtonPosition();
        return;
    } else {
        if (pauseMenuPosition > 0.0f) {
            pauseMenuPosition = std::max(pauseMenuPosition - 4.0f * deltaTime, 0.0f);
        }
        if (settingsMenuPosition > 0.0f) {
            settingsMenuPosition = std::max(settingsMenuPosition - SETTINGS_ANIMATION_SPEED * deltaTime, 0.0f);
        }
    }
    
    
    if (game.is_music_enabled() && levelMusic.getStatus() != sf::Sound::Playing && !isLevelFinished && !isDeathAnimationActive) {
        levelMusic.play();
    }
    
    
    if (isDeathAnimationActive) {
        updateDeathEffects(deltaTime);
    }
    
    if (isLevelFinished) {
        finishAnimationTimer += deltaTime;
        
        
        allDiamondsCollected = true;
        for (const auto& diamond : diamonds) {
            if (!diamond.isCollected()) {
                allDiamondsCollected = false;
                break;
            }
        }
        
        
        timeUnder30Seconds = finishTime.asSeconds() < 30.0f;
    }
    
    sf::Vector2f firePos = fireCharacter.getPosition();
    sf::Vector2f waterPos = waterCharacter.getPosition();
    
    
    for (auto& door : doors) {
        sf::FloatRect doorBounds = door.getGlobalBounds();
        
        
        float distToWater = std::sqrt(
            std::pow(waterPos.x - (doorBounds.left + doorBounds.width/2), 2) +
            std::pow(waterPos.y - (doorBounds.top + doorBounds.height/2), 2)
        );
        
        float distToFire = std::sqrt(
            std::pow(firePos.x - (doorBounds.left + doorBounds.width/2), 2) +
            std::pow(firePos.y - (doorBounds.top + doorBounds.height/2), 2)
        );
        
        
        if (door.getType() == DoorType::Water && distToWater <= 100.f) {
            door.tryInteract(true);
        } else if (door.getType() == DoorType::Fire && distToFire <= 100.f) {
            door.tryInteract(false);
        } else {
            door.close();
        }
        
        door.update(deltaTime);
    }
    
    
    if (!isLevelFinished) {
        bool allDoorsOpen = true;
        for (const auto& door : doors) {
            if (!door.isOpened()) {
                allDoorsOpen = false;
                break;
            }
        }
        
        if (allDoorsOpen && allDiamondsCollected) {
            isLevelFinished = true;
            if (game.is_sound_enabled()) {
                levelFinishSound.play();
            }
            levelMusic.stop();
            finishTime = levelTimer.getElapsedTime();
            
            
            if (currentMapFile == "data/maps/map_new_1.bnd") {
                std::cout << "Completing level 1, setting max_level to 1" << std::endl;
                game.set_max_level(1);
                std::cout << "Current max_level: " << game.get_max_level() << std::endl;
            }
            
            
            isFadingOut = true;
            fadeOutProgress = 0.0f;
            returnToMenu = true;
        }
    }
    
    
    for (auto& lever : levers) {
        
        if (lever.contains(firePos)) {
            if (!lever.wasPreviouslyTriggered() && lever.checkAndUpdateApproach(firePos)) {
                lever.toggle();
                lever.setTriggered();
                if (game.is_sound_enabled() && buttonClickSound.getStatus() != sf::Sound::Playing) {
                    buttonClickSound.play();
                }
            }
        }
        
        if (lever.contains(waterPos)) {
            if (!lever.wasPreviouslyTriggered() && lever.checkAndUpdateApproach(waterPos)) {
                lever.toggle();
                lever.setTriggered();
                if (game.is_sound_enabled() && buttonClickSound.getStatus() != sf::Sound::Playing) {
                    buttonClickSound.play();
                }
            }
        }
        
        lever.update(deltaTime);
    }
    
    
    for (size_t i = 0; i < leverPlatforms.size(); i++) {
        auto& [platform, leverIndex] = leverPlatforms[i];
        if (leverIndex < levers.size()) {
            platform.update(deltaTime, levers[leverIndex].isActivated());
        }
    }
    
    
    for (auto& button : buttons) {
        button.setPressed(button.contains(firePos) || button.contains(waterPos));
        button.update(deltaTime);
    }
    
    for (auto& [platform, buttonIndices] : buttonPlatforms) {
        bool anyButtonPressed = false;
        for (size_t index : buttonIndices) {
            if (index < buttons.size() && buttons[index].isPressed()) {
                anyButtonPressed = true;
                break;
            }
        }
        platform.update(deltaTime, anyButtonPressed);
    }
    
    if (!isDeathAnimationActive && !isFadingOut && !isPaused && !isLevelFinished) {
        
        sf::Time elapsed = levelTimer.getElapsedTime();
        int minutes = static_cast<int>(elapsed.asSeconds()) / 60;
        int seconds = static_cast<int>(elapsed.asSeconds()) % 60;
        
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << minutes << ":" 
           << std::setfill('0') << std::setw(2) << seconds;
        timerText.setString(ss.str());
        
        
        sf::FloatRect textBounds = timerText.getLocalBounds();
        timerText.setOrigin(textBounds.width / 2.0f, 0);
        timerText.setPosition(GameConstants::WINDOW_WIDTH / 2.0f, 10.0f);
    }
    
    if (isFadingOut) {
        fadeOutProgress = std::min(fadeOutProgress + FADE_SPEED * deltaTime, 1.0f);
        
        if (fadeOutProgress >= 1.0f) {
            if (returnToMenu) {
                if (isLevelFinished) {
                    game.change_state(new LevelSelectState(game));
                } else {
                    game.change_state(new MenuState(game));
                }
            } else {
                resetLevel();
                init(currentMapFile);
            }
            return;
        }
    }
    
    
    if (!isDeathAnimationActive && !isFadingOut && !isPaused && !isLevelFinished) {
        updateFireCharacter(deltaTime);
        updateWaterCharacter(deltaTime);
        checkDiamonds();
    }
}

void Level::updateLevers(float deltaTime) {
    for (auto& lever : levers) {
        lever.update(deltaTime);
    }
    
    
    for (auto& [platform, leverIndex] : leverPlatforms) {
        platform.update(deltaTime, levers[leverIndex].isActivated());
    }
}

void Level::drawLevers(sf::RenderWindow& window) {
    
    for (const auto& [platform, leverIndex] : leverPlatforms) {
        platform.draw(window);
    }
    
    
    for (const auto& lever : levers) {
        lever.draw(window);
    }
}

bool Level::canCharacterInteractWithPlatform(const Bound_box& boundary, bool isWaterCharacter) const {
    if (boundary.type == Bound_box::Bound_type::SOLID || 
        boundary.type == Bound_box::Bound_type::PLATFORM) {
        return true;
    }
    
    if (boundary.type == Bound_box::Bound_type::WATER_PLATFORM) {
        return isWaterCharacter;
    }
    
    if (boundary.type == Bound_box::Bound_type::LAVA_PLATFORM) {
        return !isWaterCharacter;
    }
    
    return false;
}

bool Level::isDeadlyPlatform(const Bound_box& boundary, bool isWaterCharacter) const {
    if (boundary.type == Bound_box::Bound_type::POISON_PLATFORM) {
        return true;
    }
    
    if (boundary.type == Bound_box::Bound_type::WATER_PLATFORM && !isWaterCharacter) {
        return true;
    }
    
    if (boundary.type == Bound_box::Bound_type::LAVA_PLATFORM && isWaterCharacter) {
        return true;
    }
    
    return false;
}

bool Level::canCharacterInteractWithPlatform(const sf::FloatRect& characterBounds, bool isWaterCharacter) {
    
    for (const auto& boundary : boundaries) {
        if (!canCharacterInteractWithPlatform(boundary, isWaterCharacter)) continue;
        
        sf::FloatRect boundaryRect(
            boundary.point1.x * pixels_per_meter,
            boundary.point1.y * pixels_per_meter,
            (boundary.point2.x - boundary.point1.x) * pixels_per_meter,
            (boundary.point2.y - boundary.point1.y) * pixels_per_meter
        );
        
        if (characterBounds.intersects(boundaryRect)) {
            return true;
        }
    }
    
    
    for (const auto& [platform, leverIndex] : leverPlatforms) {
        if (characterBounds.intersects(platform.getBounds())) {
            return true;  
        }
    }
    
    
    for (const auto& [platform, buttonIndices] : buttonPlatforms) {
        if (characterBounds.intersects(platform.getBounds())) {
            return true;  
        }
    }
    
    return false;
}

void Level::updateFireCharacter(float deltaTime) {
    if (!fireCharacter.isAlive()) return;
    
    fireCharacter.update(deltaTime);  
    
    sf::Vector2f pos = fireCharacter.getPosition();
    sf::Vector2f newPos = pos;
    
    const float CHARACTER_WIDTH = 20.0f;
    const float CHARACTER_HEIGHT = 30.0f;
    const float GROUND_SNAP_DISTANCE = 2.0f;
    const float HEIGHT_TOLERANCE = 5.0f;
    
    onGround = false;
    float lowestGroundY = std::numeric_limits<float>::max();
    
    
    for (const auto& boundary : boundaries) {
        if (!canCharacterInteractWithPlatform(boundary, false)) continue;
        
        sf::FloatRect boundaryRect(
            boundary.point1.x * pixels_per_meter,
            boundary.point1.y * pixels_per_meter,
            (boundary.point2.x - boundary.point1.x) * pixels_per_meter,
            (boundary.point2.y - boundary.point1.y) * pixels_per_meter
        );
        
        if (pos.x + CHARACTER_WIDTH/2 >= boundaryRect.left && 
            pos.x - CHARACTER_WIDTH/2 <= boundaryRect.left + boundaryRect.width) {
            
            if (boundaryRect.top >= pos.y + CHARACTER_HEIGHT/2 && 
                boundaryRect.top < pos.y + CHARACTER_HEIGHT/2 + GROUND_SNAP_DISTANCE + HEIGHT_TOLERANCE && 
                boundaryRect.top < lowestGroundY) {
                lowestGroundY = boundaryRect.top;
                onGround = true;
                lastGroundY = boundaryRect.top - CHARACTER_HEIGHT/2;
            }
        }
    }
    
    
    for (const auto& [platform, leverIndex] : leverPlatforms) {
        sf::FloatRect platformBounds = platform.getBounds();
        
        if (pos.x + CHARACTER_WIDTH/2 >= platformBounds.left && 
            pos.x - CHARACTER_WIDTH/2 <= platformBounds.left + platformBounds.width) {
            
            if (platformBounds.top >= pos.y + CHARACTER_HEIGHT/2 && 
                platformBounds.top < pos.y + CHARACTER_HEIGHT/2 + GROUND_SNAP_DISTANCE + HEIGHT_TOLERANCE && 
                platformBounds.top < lowestGroundY) {
                lowestGroundY = platformBounds.top;
                onGround = true;
                lastGroundY = platformBounds.top - CHARACTER_HEIGHT/2;
            }
        }
    }
    
    if (onGround) {
        newPos.y = lastGroundY;
        verticalSpeed = 0;
    }
    
    sf::Vector2f moveStep;
    if (isMovingLeft) {
        moveStep.x = -moveSpeed * deltaTime;
        fireCharacter.setRunning();
        fireCharacter.setDirection(false);
    } else if (isMovingRight) {
        moveStep.x = moveSpeed * deltaTime;
        fireCharacter.setRunning();
        fireCharacter.setDirection(true);
    } else {
        moveStep.x = 0;
        fireCharacter.setIdle();
    }
    
    
    sf::FloatRect horizontalCheck(
        pos.x + moveStep.x - CHARACTER_WIDTH/2,
        pos.y - CHARACTER_HEIGHT/2 + HEIGHT_TOLERANCE,
        CHARACTER_WIDTH,
        CHARACTER_HEIGHT - 2 * HEIGHT_TOLERANCE
    );
    
    bool hasHorizontalCollision = canCharacterInteractWithPlatform(horizontalCheck, false);
    
    if (!hasHorizontalCollision) {
        newPos.x += moveStep.x;
    }
    
    if (isJumping && onGround) {
        verticalSpeed = jumpSpeed;
        onGround = false;
        isJumping = false;
    }
    
    if (!onGround) {
        verticalSpeed += gravity * deltaTime;
        float verticalStep = verticalSpeed * deltaTime;
        
        sf::FloatRect verticalCheck(
            newPos.x - CHARACTER_WIDTH/2,
            newPos.y + verticalStep - CHARACTER_HEIGHT/2,
            CHARACTER_WIDTH,
            CHARACTER_HEIGHT
        );
        
        bool hasVerticalCollision = canCharacterInteractWithPlatform(verticalCheck, false);
        
        if (hasVerticalCollision) {
            
            float closestY = std::numeric_limits<float>::max();
            for (const auto& boundary : boundaries) {
                if (!canCharacterInteractWithPlatform(boundary, false)) continue;
                
                sf::FloatRect boundaryRect(
                    boundary.point1.x * pixels_per_meter,
                    boundary.point1.y * pixels_per_meter,
                    (boundary.point2.x - boundary.point1.x) * pixels_per_meter,
                    (boundary.point2.y - boundary.point1.y) * pixels_per_meter
                );
                
                if (verticalCheck.intersects(boundaryRect)) {
                    if (verticalSpeed > 0 && boundaryRect.top < closestY) {
                        closestY = boundaryRect.top;
                        verticalStep = boundaryRect.top - (pos.y + CHARACTER_HEIGHT/2);
                        verticalSpeed = 0;
                        onGround = true;
                    } else if (verticalSpeed < 0 && boundaryRect.top + boundaryRect.height > -closestY) {
                        closestY = -(boundaryRect.top + boundaryRect.height);
                        verticalStep = (boundaryRect.top + boundaryRect.height) - (pos.y - CHARACTER_HEIGHT/2);
                        verticalSpeed = 0;
                    }
                }
            }
            
            
            for (const auto& [platform, leverIndex] : leverPlatforms) {
                sf::FloatRect platformRect = platform.getBounds();
                if (verticalCheck.intersects(platformRect)) {
                    if (verticalSpeed > 0 && platformRect.top < closestY) {
                        closestY = platformRect.top;
                        verticalStep = platformRect.top - (pos.y + CHARACTER_HEIGHT/2);
                        verticalSpeed = 0;
                        onGround = true;
                    } else if (verticalSpeed < 0 && platformRect.top + platformRect.height > -closestY) {
                        closestY = -(platformRect.top + platformRect.height);
                        verticalStep = (platformRect.top + platformRect.height) - (pos.y - CHARACTER_HEIGHT/2);
                        verticalSpeed = 0;
                    }
                }
            }
            
            
            for (const auto& [platform, buttonIndices] : buttonPlatforms) {
                sf::FloatRect platformRect = platform.getBounds();
                if (verticalCheck.intersects(platformRect)) {
                    if (verticalSpeed > 0 && platformRect.top < closestY) {
                        closestY = platformRect.top;
                        verticalStep = platformRect.top - (pos.y + CHARACTER_HEIGHT/2);
                        verticalSpeed = 0;
                        onGround = true;
                    } else if (verticalSpeed < 0 && platformRect.top + platformRect.height > -closestY) {
                        closestY = -(platformRect.top + platformRect.height);
                        verticalStep = (platformRect.top + platformRect.height) - (pos.y - CHARACTER_HEIGHT/2);
                        verticalSpeed = 0;
                    }
                }
            }
        }
        
        newPos.y += verticalStep;
    }
    
    sf::FloatRect characterBounds(
        newPos.x - CHARACTER_WIDTH/2,
        newPos.y - CHARACTER_HEIGHT/2,
        CHARACTER_WIDTH,
        CHARACTER_HEIGHT
    );
    
    for (const auto& boundary : boundaries) {
        sf::FloatRect boundaryRect(
            boundary.point1.x * pixels_per_meter,
            boundary.point1.y * pixels_per_meter,
            (boundary.point2.x - boundary.point1.x) * pixels_per_meter,
            (boundary.point2.y - boundary.point1.y) * pixels_per_meter
        );
        
        if (characterBounds.intersects(boundaryRect) && isDeadlyPlatform(boundary, false)) {
            fireCharacter.kill();
            return;
        }
    }
    
    
    for (auto& block : blocks) {
        sf::FloatRect blockBounds = block.getBounds();
        sf::FloatRect characterBounds(
            newPos.x - CHARACTER_WIDTH/2,
            newPos.y - CHARACTER_HEIGHT/2,
            CHARACTER_WIDTH,
            CHARACTER_HEIGHT
        );
        
        if (blockBounds.intersects(characterBounds)) {
            
            sf::Vector2f oldPos = fireCharacter.getPosition();
            sf::Vector2f blockCenter(blockBounds.left + blockBounds.width/2, blockBounds.top + blockBounds.height/2);
            sf::Vector2f direction = oldPos - blockCenter;
            
            
            if (std::abs(direction.x) > std::abs(direction.y)) {
                
                if (direction.x > 0) {
                    newPos.x = blockBounds.left + blockBounds.width + CHARACTER_WIDTH/2;
                    
                    if (isMovingLeft) {
                        block.applyForce(sf::Vector2f(-PushableBlock::PUSH_FORCE, 0), deltaTime);
                    }
                } else {
                    newPos.x = blockBounds.left - CHARACTER_WIDTH/2;
                    
                    if (isMovingRight) {
                        block.applyForce(sf::Vector2f(PushableBlock::PUSH_FORCE, 0), deltaTime);
                    }
                }
            } else {
                
                if (direction.y > 0) {
                    newPos.y = blockBounds.top + blockBounds.height + CHARACTER_HEIGHT/2;
                } else {
                    newPos.y = blockBounds.top - CHARACTER_HEIGHT/2;
                    if (isJumping) {
                        isJumping = false;
                    }
                    onGround = true;
                }
                verticalSpeed = 0;
            }
        }
    }
    
    fireCharacter.setPosition(newPos);
}

void Level::updateWaterCharacter(float deltaTime) {
    if (!waterCharacter.isAlive()) return;
    
    waterCharacter.update(deltaTime);  
    
    sf::Vector2f pos = waterCharacter.getPosition();
    sf::Vector2f newPos = pos;
    
    const float CHARACTER_WIDTH = 20.0f;
    const float CHARACTER_HEIGHT = 30.0f;
    const float GROUND_SNAP_DISTANCE = 2.0f;
    const float HEIGHT_TOLERANCE = 5.0f; 
    
    bool wasOnGround = waterOnGround;
    float prevVerticalSpeed = waterVerticalSpeed;
    
    waterOnGround = false;
    float lowestGroundY = std::numeric_limits<float>::max();
    
    sf::FloatRect groundCheck(
        pos.x - CHARACTER_WIDTH/2,
        pos.y + CHARACTER_HEIGHT/2,
        CHARACTER_WIDTH,
        GROUND_SNAP_DISTANCE
    );
    
    
    for (const auto& boundary : boundaries) {
        if (!canCharacterInteractWithPlatform(boundary, true)) {
            continue;
        }
        
        sf::FloatRect boundaryRect(
            boundary.point1.x * pixels_per_meter,
            boundary.point1.y * pixels_per_meter,
            (boundary.point2.x - boundary.point1.x) * pixels_per_meter,
            (boundary.point2.y - boundary.point1.y) * pixels_per_meter
        );
        
        if (pos.x + CHARACTER_WIDTH/2 >= boundaryRect.left && 
            pos.x - CHARACTER_WIDTH/2 <= boundaryRect.left + boundaryRect.width) {
            
            if (boundaryRect.top >= pos.y + CHARACTER_HEIGHT/2 && 
                boundaryRect.top < pos.y + CHARACTER_HEIGHT/2 + GROUND_SNAP_DISTANCE + HEIGHT_TOLERANCE && 
                boundaryRect.top < lowestGroundY) {
                lowestGroundY = boundaryRect.top;
                waterOnGround = true;
                waterLastGroundY = boundaryRect.top - CHARACTER_HEIGHT/2;
            }
        }
    }
    
    
    for (const auto& [platform, leverIndex] : leverPlatforms) {
        sf::FloatRect platformBounds = platform.getBounds();
        
        if (pos.x + CHARACTER_WIDTH/2 >= platformBounds.left && 
            pos.x - CHARACTER_WIDTH/2 <= platformBounds.left + platformBounds.width) {
            
            if (platformBounds.top >= pos.y + CHARACTER_HEIGHT/2 && 
                platformBounds.top < pos.y + CHARACTER_HEIGHT/2 + GROUND_SNAP_DISTANCE + HEIGHT_TOLERANCE && 
                platformBounds.top < lowestGroundY) {
                lowestGroundY = platformBounds.top;
                waterOnGround = true;
                waterLastGroundY = platformBounds.top - CHARACTER_HEIGHT/2;
            }
        }
    }
    
    
    for (const auto& [platform, buttonIndices] : buttonPlatforms) {
        sf::FloatRect platformBounds = platform.getBounds();
        
        if (pos.x + CHARACTER_WIDTH/2 >= platformBounds.left && 
            pos.x - CHARACTER_WIDTH/2 <= platformBounds.left + platformBounds.width) {
            
            if (platformBounds.top >= pos.y + CHARACTER_HEIGHT/2 && 
                platformBounds.top < pos.y + CHARACTER_HEIGHT/2 + GROUND_SNAP_DISTANCE + HEIGHT_TOLERANCE && 
                platformBounds.top < lowestGroundY) {
                lowestGroundY = platformBounds.top;
                waterOnGround = true;
                waterLastGroundY = platformBounds.top - CHARACTER_HEIGHT/2;
            }
        }
    }
    
    if (waterOnGround) {
        newPos.y = waterLastGroundY;
        waterVerticalSpeed = 0;
    }
    
    sf::Vector2f moveStep;
    if (isWaterMovingLeft) {
        moveStep.x = -moveSpeed * deltaTime;
        waterCharacter.setRunning();
        waterCharacter.setDirection(false);  
    }
    else if (isWaterMovingRight) {
        moveStep.x = moveSpeed * deltaTime;
        waterCharacter.setRunning();
        waterCharacter.setDirection(true);   
    }
    else {
        moveStep.x = 0;
        waterCharacter.setIdle();  
    }
    
    sf::FloatRect horizontalCheck(
        pos.x + moveStep.x - CHARACTER_WIDTH/2,
        pos.y - CHARACTER_HEIGHT/2 + HEIGHT_TOLERANCE, 
        CHARACTER_WIDTH,
        CHARACTER_HEIGHT - HEIGHT_TOLERANCE 
    );
    
    bool hasHorizontalCollision = canCharacterInteractWithPlatform(horizontalCheck, true);
    
    if (!hasHorizontalCollision) {
        newPos.x += moveStep.x;
    }
    
    if (isWaterJumping && waterOnGround) {
        waterVerticalSpeed = jumpSpeed;
        waterOnGround = false;
    }
    
    if (!waterOnGround) {
        waterVerticalSpeed += gravity * deltaTime;
        float verticalStep = waterVerticalSpeed * deltaTime;
        
        sf::FloatRect verticalCheck(
            newPos.x - CHARACTER_WIDTH/2,
            newPos.y + verticalStep - CHARACTER_HEIGHT/2,
            CHARACTER_WIDTH,
            CHARACTER_HEIGHT
        );
        
        bool hasVerticalCollision = canCharacterInteractWithPlatform(verticalCheck, true);
        
        if (hasVerticalCollision) {
            
            float closestY = std::numeric_limits<float>::max();
            for (const auto& boundary : boundaries) {
                if (!canCharacterInteractWithPlatform(boundary, true)) continue;
                
                sf::FloatRect boundaryRect(
                    boundary.point1.x * pixels_per_meter,
                    boundary.point1.y * pixels_per_meter,
                    (boundary.point2.x - boundary.point1.x) * pixels_per_meter,
                    (boundary.point2.y - boundary.point1.y) * pixels_per_meter
                );
                
                if (verticalCheck.intersects(boundaryRect)) {
                    if (waterVerticalSpeed > 0 && boundaryRect.top < closestY) {
                        closestY = boundaryRect.top;
                        verticalStep = boundaryRect.top - (pos.y + CHARACTER_HEIGHT/2);
                        waterVerticalSpeed = 0;
                        waterOnGround = true;
                    } else if (waterVerticalSpeed < 0 && boundaryRect.top + boundaryRect.height > -closestY) {
                        closestY = -(boundaryRect.top + boundaryRect.height);
                        verticalStep = (boundaryRect.top + boundaryRect.height) - (pos.y - CHARACTER_HEIGHT/2);
                        waterVerticalSpeed = 0;
                    }
                }
            }
            
            
            for (const auto& [platform, leverIndex] : leverPlatforms) {
                sf::FloatRect platformRect = platform.getBounds();
                if (verticalCheck.intersects(platformRect)) {
                    if (waterVerticalSpeed > 0 && platformRect.top < closestY) {
                        closestY = platformRect.top;
                        verticalStep = platformRect.top - (pos.y + CHARACTER_HEIGHT/2);
                        waterVerticalSpeed = 0;
                        waterOnGround = true;
                    } else if (waterVerticalSpeed < 0 && platformRect.top + platformRect.height > -closestY) {
                        closestY = -(platformRect.top + platformRect.height);
                        verticalStep = (platformRect.top + platformRect.height) - (pos.y - CHARACTER_HEIGHT/2);
                        waterVerticalSpeed = 0;
                    }
                }
            }
            
            
            for (const auto& [platform, buttonIndices] : buttonPlatforms) {
                sf::FloatRect platformRect = platform.getBounds();
                if (verticalCheck.intersects(platformRect)) {
                    if (waterVerticalSpeed > 0 && platformRect.top < closestY) {
                        closestY = platformRect.top;
                        verticalStep = platformRect.top - (pos.y + CHARACTER_HEIGHT/2);
                        waterVerticalSpeed = 0;
                        waterOnGround = true;
                    } else if (waterVerticalSpeed < 0 && platformRect.top + platformRect.height > -closestY) {
                        closestY = -(platformRect.top + platformRect.height);
                        verticalStep = (platformRect.top + platformRect.height) - (pos.y - CHARACTER_HEIGHT/2);
                        waterVerticalSpeed = 0;
                    }
                }
            }
        }
        
        newPos.y += verticalStep;
    }
    
    sf::FloatRect characterBounds(
        newPos.x - CHARACTER_WIDTH/2,
        newPos.y - CHARACTER_HEIGHT/2,
        CHARACTER_WIDTH,
        CHARACTER_HEIGHT
    );
    
    for (const auto& boundary : boundaries) {
        sf::FloatRect boundaryRect(
            boundary.point1.x * pixels_per_meter,
            boundary.point1.y * pixels_per_meter,
            (boundary.point2.x - boundary.point1.x) * pixels_per_meter,
            (boundary.point2.y - boundary.point1.y) * pixels_per_meter
        );
        
        if (characterBounds.intersects(boundaryRect) && isDeadlyPlatform(boundary, true)) {
            waterCharacter.kill(); 
            return;
        }
    }
    
    
    for (auto& block : blocks) {  
        sf::FloatRect blockBounds = block.getBounds();
        sf::FloatRect characterBounds(
            newPos.x - CHARACTER_WIDTH/2,
            newPos.y - CHARACTER_HEIGHT/2,
            CHARACTER_WIDTH,
            CHARACTER_HEIGHT
        );
        
        if (blockBounds.intersects(characterBounds)) {
            
            sf::Vector2f oldPos = waterCharacter.getPosition();
            sf::Vector2f blockCenter(blockBounds.left + blockBounds.width/2, blockBounds.top + blockBounds.height/2);
            sf::Vector2f direction = oldPos - blockCenter;
            
            
            if (std::abs(direction.x) > std::abs(direction.y)) {
                
                if (direction.x > 0) {
                    newPos.x = blockBounds.left + blockBounds.width + CHARACTER_WIDTH/2;
                    
                    if (isWaterMovingLeft) {
                        block.applyForce(sf::Vector2f(-PushableBlock::PUSH_FORCE, 0), deltaTime);
                    }
                } else {
                    newPos.x = blockBounds.left - CHARACTER_WIDTH/2;
                    
                    if (isWaterMovingRight) {
                        block.applyForce(sf::Vector2f(PushableBlock::PUSH_FORCE, 0), deltaTime);
                    }
                }
            } else {
                
                if (direction.y > 0) {
                    newPos.y = blockBounds.top + blockBounds.height + CHARACTER_HEIGHT/2;
                } else {
                    newPos.y = blockBounds.top - CHARACTER_HEIGHT/2;
                    waterOnGround = true;
                    waterLastGroundY = newPos.y;
                }
            }
        }
    }
    
    waterCharacter.setPosition(newPos);
}

void Level::checkDiamonds() {
    
    bool waterAtDoor = false;
    bool fireAtDoor = false;
    for (const auto& door : doors) {
        sf::FloatRect doorBounds = door.getGlobalBounds();
        sf::FloatRect waterBounds = waterCharacter.getGlobalBounds();
        sf::FloatRect fireBounds = fireCharacter.getGlobalBounds();

        if (door.getType() == DoorType::Water && doorBounds.intersects(waterBounds)) {
            waterAtDoor = true;
        }
        if (door.getType() == DoorType::Fire && doorBounds.intersects(fireBounds)) {
            fireAtDoor = true;
        }
    }

    
    if (waterAtDoor && fireAtDoor && !isLevelFinished) {
        bool allDiamondsCollected = true;
        for (const auto& diamond : diamonds) {
            if (!diamond.isCollected()) {
                allDiamondsCollected = false;
                break;
            }
        }

        if (allDiamondsCollected) {
            isLevelFinished = true;
            if (game.is_sound_enabled()) {
                levelFinishSound.play();
            }
            levelMusic.stop();
            finishTime = levelTimer.getElapsedTime();
        }
    }

    
    for (auto& diamond : diamonds) {
        if (!diamond.isCollected()) {
            sf::FloatRect diamondBounds = diamond.getGlobalBounds();
            
            
            if (diamondBounds.intersects(waterCharacter.getGlobalBounds()) && diamond.canCollect(true)) {
                diamond.collect();
                if (game.is_sound_enabled() && diamondSound.getStatus() != sf::Sound::Playing) {
                    diamondSound.play();
                }
            }
            
            
            if (diamondBounds.intersects(fireCharacter.getGlobalBounds()) && diamond.canCollect(false)) {
                diamond.collect();
                if (game.is_sound_enabled() && diamondSound.getStatus() != sf::Sound::Playing) {
                    diamondSound.play();
                }
            }
        }
    }
}

void Level::draw(sf::RenderWindow& window, sf::Vector2f viewPos, bool debug) {
    
    sf::View currentView = window.getView();
    sf::Vector2f viewSize = currentView.getSize();
    sf::Vector2f viewCenter = currentView.getCenter();
    
    drawableObject.setPosition(viewPos);
    window.draw(drawableObject);

    for (auto& door : doors) {
        door.draw(window);
    }
    
    for (auto& diamond : diamonds) {
        diamond.draw(window, viewPos);
    }
    
    for (auto& hint : hints) {
        hint.draw(window, viewPos, waterCharacter.getPosition(), fireCharacter.getPosition());
    }

    drawLevers(window);

    for (const auto& button : buttons) {
        button.draw(window);
    }
    for (const auto& [platform, _] : buttonPlatforms) {
        platform.draw(window);
    }

    fireCharacter.draw(window);
    waterCharacter.draw(window);
    
    for (const auto& block : blocks) {
        block.draw(window);
    }
    
    if (isDeathAnimationActive) {
        drawDeathEffects(window);
    }
    
    window.setView(window.getDefaultView());
    
    if (!isDeathAnimationActive && pauseMenuPosition == 0.0f && !isFadingOut && !isLevelFinished) {
        window.draw(pauseButton);
    }
    
    if (pauseMenuPosition > 0.0f) {
        
        sf::Color overlayColor = sf::Color(0, 0, 0, static_cast<sf::Uint8>(pauseMenuPosition * 128));
        darkOverlay.setFillColor(overlayColor);
        window.draw(darkOverlay);
        
        float centerX = GameConstants::WINDOW_WIDTH / 2.0f;
        float baseY = GameConstants::WINDOW_HEIGHT * 1.2f; 
        float targetY = GameConstants::WINDOW_HEIGHT * 0.4f; 
        float currentY = baseY + (targetY - baseY) * pauseMenuPosition; 
        
        
        sf::Color menuColor = sf::Color(255, 255, 255, 255);
        pauseMenuBackground.setColor(menuColor);
        pauseMenuBackground.setPosition(centerX, currentY);
        window.draw(pauseMenuBackground);
        
        
        menuButtonSprite.setColor(menuColor);
        retryButtonSprite.setColor(menuColor);
        resumeButton.setColor(menuColor);
        settingsButton.setColor(menuColor);
        
        float buttonOffsetY = currentY;
        float spacing = 80.0f;
        float horizontalSpacing = 100.0f;
        
        menuButtonSprite.setPosition(centerX - horizontalSpacing, buttonOffsetY);
        retryButtonSprite.setPosition(centerX + horizontalSpacing, buttonOffsetY);
        resumeButton.setPosition(centerX, buttonOffsetY + spacing);
        
        sf::FloatRect menuBounds = pauseMenuBackground.getGlobalBounds();
        float settingsX = menuBounds.left + menuBounds.width - 80.0f;
        float settingsY = menuBounds.top + 80.0f;
        settingsButton.setPosition(settingsX, settingsY);
        
        window.draw(menuButtonSprite);
        window.draw(retryButtonSprite);
        window.draw(resumeButton);
        window.draw(settingsButton);
    }
    
    if (settingsMenuActive && settingsMenuPosition > 0.0f) {
        sf::FloatRect menuRect = settingsMenuSprite.getLocalBounds();
        float startY = GameConstants::WINDOW_HEIGHT + menuRect.height;
        float endY = GameConstants::WINDOW_HEIGHT * 0.4f;
        float currentY = startY + (endY - startY) * settingsMenuPosition;
        
        
        sf::Color menuColor = sf::Color(255, 255, 255, 255);
        settingsMenuSprite.setColor(menuColor);
        settingsMenuSprite.setPosition(GameConstants::WINDOW_WIDTH/2, currentY);
        
        
        musicButton.setColor(menuColor);
        soundButton.setColor(menuColor);
        okButton.setColor(menuColor);
        
        float menuCenterY = currentY;
        musicButton.setPosition(GameConstants::WINDOW_WIDTH/2 - 50.0f, menuCenterY);
        soundButton.setPosition(GameConstants::WINDOW_WIDTH/2 + 50.0f, menuCenterY);
        okButton.setPosition(GameConstants::WINDOW_WIDTH/2, menuCenterY + 80.0f);
        
        window.draw(settingsMenuSprite);
        window.draw(musicButton);
        window.draw(soundButton);
        window.draw(okButton);
    }
    
    if (isFadingOut) {
        sf::Color fadeColor = sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeOutProgress * 255));
        darkOverlay.setFillColor(fadeColor);
        window.draw(darkOverlay);
    }
    
    if (isLevelFinished) {
        sf::Color overlayColor = sf::Color(0, 0, 0, 128);
        darkOverlay.setFillColor(overlayColor);
        window.draw(darkOverlay);

        
        float windowWidth = static_cast<float>(GameConstants::WINDOW_WIDTH);
        float windowHeight = static_cast<float>(GameConstants::WINDOW_HEIGHT);

        
        sf::FloatRect finishBounds = levelFinishBackground.getGlobalBounds();
        levelFinishBackground.setOrigin(finishBounds.width / 2.f, finishBounds.height / 2.f);
        levelFinishBackground.setPosition(windowWidth / 2.f, windowHeight / 2.f);
        window.draw(levelFinishBackground);

        
        int minutes = static_cast<int>(finishTime.asSeconds()) / 60;
        int seconds = static_cast<int>(finishTime.asSeconds()) % 60;
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << minutes << ":" 
           << std::setfill('0') << std::setw(2) << seconds;
        timerText.setString(ss.str());
        
        
        sf::FloatRect textBounds = timerText.getLocalBounds();
        timerText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
        timerText.setPosition(windowWidth / 2.f, windowHeight / 2.f - 150.f);
        window.draw(timerText);

        
        float centerX = windowWidth / 2.0f;
        float baseY = windowHeight / 2.0f;
        float iconSpacingY = 60.0f;
        float iconStartX = centerX - 200.0f;  

        
        float pairProgress = std::min(1.0f, std::max(0.0f, (finishAnimationTimer - 0.0f) / 0.5f));
        float diamondsProgress = std::min(1.0f, std::max(0.0f, (finishAnimationTimer - 0.5f) / 0.5f));
        float timeProgress = std::min(1.0f, std::max(0.0f, (finishAnimationTimer - 1.0f) / 0.5f));

        bool allAnimationsComplete = (pairProgress >= 1.0f && diamondsProgress >= 1.0f && timeProgress >= 1.0f);
        
        
        arrowSprite.setPosition(centerX + 100.0f, baseY);
        if (allAnimationsComplete) {
            diamondFullSprite.setPosition(centerX + 200.0f, baseY);
            window.draw(diamondFullSprite);
        } else {
            diamondEmptySprite.setPosition(centerX + 200.0f, baseY);
            window.draw(diamondEmptySprite);
        }
        window.draw(arrowSprite);

        
        if (pairProgress > 0.0f) {
            float startX = -200.0f;
            float endX = iconStartX;
            float currentX = startX + (endX - startX) * pairProgress;
            pairSprite.setPosition(currentX, baseY - iconSpacingY);
            window.draw(pairSprite);

            if (pairProgress >= 1.0f) {
                okIcon1Sprite.setPosition(currentX + 150.0f, baseY - iconSpacingY);
                window.draw(okIcon1Sprite);
            }
        }

        if (diamondsProgress > 0.0f) {
            float startX = -200.0f;
            float endX = iconStartX;
            float currentX = startX + (endX - startX) * diamondsProgress;
            diamondsSprite.setPosition(currentX, baseY);
            window.draw(diamondsSprite);

            if (diamondsProgress >= 1.0f) {
                if (allDiamondsCollected) {
                    okIcon2Sprite.setPosition(currentX + 150.0f, baseY);
                    window.draw(okIcon2Sprite);
                } else {
                    xmarkIcon2Sprite.setPosition(currentX + 150.0f, baseY);
                    window.draw(xmarkIcon2Sprite);
                }
            }
        }

        if (timeProgress > 0.0f) {
            float startX = -200.0f;
            float endX = iconStartX;
            float currentX = startX + (endX - startX) * timeProgress;
            timeSprite.setPosition(currentX, baseY + iconSpacingY);
            window.draw(timeSprite);

            if (timeProgress >= 1.0f) {
                if (timeUnder30Seconds) {
                    okIcon3Sprite.setPosition(currentX + 150.0f, baseY + iconSpacingY);
                    window.draw(okIcon3Sprite);
                } else {
                    xmarkIcon3Sprite.setPosition(currentX + 150.0f, baseY + iconSpacingY);
                    window.draw(xmarkIcon3Sprite);
                }
            }
        }
    }
    
    
    
    
    
    
    
    
    
    
    if (!isDeathAnimationActive && pauseMenuPosition == 0.0f && !isFadingOut && !isLevelFinished) {
        window.draw(timerText);
    }
    
    if (isLevelFinished) {
        float buttonX = GameConstants::WINDOW_WIDTH / 2.0f;
        float buttonY = GameConstants::WINDOW_HEIGHT * 0.7f - 8.0f;  
        continueButtonSprite.setPosition(buttonX, buttonY);
        window.draw(continueButtonSprite);
    }
    
    if (darkOverlay.getFillColor().a > 0 && !isLevelFinished) {
        window.draw(darkOverlay);
    }
    
    window.setView(currentView);
}

bool Level::isPointInSprite(const sf::Vector2f& point, const sf::Sprite& sprite) const {
    return sprite.getGlobalBounds().contains(point);
}

void Level::updatePauseButtonPosition() {
    sf::Vector2u windowSize = game.get_window().getSize();
    float scale = 1.0f;  
    pauseButton.setScale(scale, scale);
    
    sf::FloatRect bounds = pauseButton.getGlobalBounds();
    pauseButton.setPosition(
        windowSize.x - bounds.width - 10.0f,  
        10.0f  
    );
}
