#pragma once

#include "GameState.h"
#include <SFML/Graphics.hpp>

class GameEngine;

class LevelSelectState : public GameState {
public:
    explicit LevelSelectState(GameEngine& game);
    
    void handle_input(const sf::Event& event) override;
    void update() override;
    void render(sf::RenderWindow& window) override;
    
private:
    GameEngine& game;
    
    sf::Texture background_texture;
    sf::Sprite background_sprite;
    
    sf::Texture back_button_texture;
    sf::Sprite back_button;

    sf::Texture level_progress_texture;
    sf::Sprite level_progress_sprite;

    int max_unlocked_level = 0;
    
    sf::RectangleShape fade_rect;
    float fade_alpha = 255.0f;
    bool is_fading_in = true;
    bool is_fading_out = false;
    const float FADE_SPEED = 510.0f;
    
    bool is_point_in_sprite(const sf::Vector2f& point, const sf::Sprite& sprite);
    void start_fade_out();
    void load_progress_image();

};
