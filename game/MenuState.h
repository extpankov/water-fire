#ifndef MENU_STATE_H_
#define MENU_STATE_H_

#include "GameState.h"
#include "LevelSelectState.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class GameEngine;

class MenuState : public GameState {
public:
    MenuState(GameEngine& game);
    ~MenuState();
    
    void handle_input(const sf::Event& event) override;
    void update() override;
    void render(sf::RenderWindow& window) override;
    
private:
    GameEngine& game;
    sf::Font font;
    sf::Text start_text;
    sf::Texture background_texture;
    sf::Sprite background_sprite;
    sf::Texture logo_texture;
    sf::Sprite logo_sprite;
    sf::Texture settings_texture;
    sf::Sprite settings_sprite;
    sf::Texture settings_menu_texture;
    sf::Sprite settings_menu_sprite;
    
    sf::Texture music_on_texture;
    sf::Texture music_off_texture;
    sf::Sprite music_button;
    sf::Texture sound_on_texture;
    sf::Texture sound_off_texture;
    sf::Sprite sound_button;
    sf::Texture ok_button_texture;
    sf::Sprite ok_button;
    bool is_music_on = true;
    bool is_sound_on = true;
    
    bool settings_menu_active = false;
    float settings_menu_position = 0.0f;
    const float SETTINGS_ANIMATION_SPEED = 4.0f;
    
    void update_button_positions();
    bool is_point_in_sprite(const sf::Vector2f& point, const sf::Sprite& sprite) const;
    bool is_point_in_text(const sf::Vector2f& point, const sf::Text& text) const;
    
    sf::Music menu_music;
};

#endif 
