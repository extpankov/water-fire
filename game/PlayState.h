#ifndef PLAY_STATE_H_
#define PLAY_STATE_H_

#include "GameState.h"
#include "Level.h"
#include <SFML/Graphics.hpp>

class GameEngine;

class PlayState : public GameState {
public:
    PlayState(GameEngine& game);
    PlayState(GameEngine& game, const std::string& mapFile);
    ~PlayState();
    
    void handle_input(const sf::Event& event) override;
    void update() override;
    void render(sf::RenderWindow& window) override;
    
private:
    void update_view();
    
    GameEngine& game;
    Level level; 
    sf::View game_view;
    
    // Fade effect
    static constexpr float FADE_SPEED = 255.0f;
    float fade_alpha;
    bool is_fading_in;
    sf::RectangleShape fade_rect;
    
    // Time measurement
    sf::Clock frameClock;
    sf::Time lastFrameTime;
};

#endif // PLAY_STATE_H_
