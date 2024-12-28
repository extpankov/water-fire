#include "MenuState.h"
#include "GameEngine.h"
#include "LevelSelectState.h"
#include "PlayState.h"
#include "GameConstants.h"
#include <iostream>
#include <SFML/Audio.hpp>

MenuState::MenuState(GameEngine& game) : game(game) {
    std::cout << "Инициализация состояния меню" << std::endl;
    
    std::cout << "Загрузка ресурсов меню..." << std::endl;
    
    if (!background_texture.loadFromFile("data/background.png")) {
        sf::Image bg;
        bg.create(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT, sf::Color(30, 30, 30));
        background_texture.loadFromImage(bg);
    }
    background_sprite.setTexture(background_texture);
    
    float scaleX = static_cast<float>(GameConstants::WINDOW_WIDTH) / background_texture.getSize().x;
    float scaleY = static_cast<float>(GameConstants::WINDOW_HEIGHT) / background_texture.getSize().y;
    background_sprite.setScale(scaleX, scaleY);
    
    if (!logo_texture.loadFromFile("data/logo.png")) {
        std::cerr << "Ошибка при загрузке логотипа: data/logo.png" << std::endl;
    } else {
        logo_sprite.setTexture(logo_texture);
        sf::FloatRect logoRect = logo_sprite.getLocalBounds();
        logo_sprite.setOrigin(logoRect.width/2, logoRect.height/2);
        logo_sprite.setPosition(GameConstants::WINDOW_WIDTH/2, GameConstants::WINDOW_HEIGHT/4);
        
        float scale = std::min(400.f / logoRect.width, 200.f / logoRect.height);
        logo_sprite.setScale(scale, scale);
    }
    
    if (!settings_texture.loadFromFile("data/settings.png")) {
        std::cerr << "Ошибка при загрузке кнопки настроек: data/settings.png" << std::endl;
    } else {
        settings_sprite.setTexture(settings_texture);
        const float PADDING = 20.f;
        const float TARGET_SIZE = 50.f;
        
        sf::FloatRect settingsRect = settings_sprite.getLocalBounds();
        settings_sprite.setOrigin(0, settingsRect.height);
        
        float scale = TARGET_SIZE / std::max(settingsRect.width, settingsRect.height);
        settings_sprite.setScale(scale, scale);
        
        settings_sprite.setPosition(PADDING, GameConstants::WINDOW_HEIGHT - PADDING);
    }
    
    if (!settings_menu_texture.loadFromFile("data/settings_menu.png")) {
        std::cerr << "Ошибка при загрузке меню настроек: data/settings_menu.png" << std::endl;
    } else {
        settings_menu_sprite.setTexture(settings_menu_texture);
        
        sf::FloatRect menuRect = settings_menu_sprite.getLocalBounds();
        settings_menu_sprite.setOrigin(menuRect.width/2, 0);
        
        settings_menu_sprite.setPosition(
            GameConstants::WINDOW_WIDTH/2,
            GameConstants::WINDOW_HEIGHT + menuRect.height
        );
        
        float scale = std::min(
            GameConstants::WINDOW_WIDTH * 0.8f / menuRect.width,
            GameConstants::WINDOW_HEIGHT * 0.8f / menuRect.height
        );
        settings_menu_sprite.setScale(scale, scale);
    }
    
    if (!music_on_texture.loadFromFile("data/settings_music_button_on.png")) {
        std::cerr << "Ошибка при загрузке музыки вкл" << std::endl;
    }
    if (!music_off_texture.loadFromFile("data/settings_music_button_off.png")) {
        std::cerr << "Ошибка при загрузке музыки выкл" << std::endl;
    }
    if (!sound_on_texture.loadFromFile("data/settings_sound_button_on.png")) {
        std::cerr << "Ошибка при загрузке звука вкл" << std::endl;
    }
    if (!sound_off_texture.loadFromFile("data/settings_sound_button_off.png")) {
        std::cerr << "Ошибка при загрузке звука выкл" << std::endl;
    }
    if (!ok_button_texture.loadFromFile("data/settings_ok_button.png")) {
        std::cerr << "Ошибка при загрузке кнопки ок" << std::endl;
    }
    
    music_button.setTexture(music_on_texture);
    
    
    sound_button.setTexture(sound_on_texture);
    
    
    ok_button.setTexture(ok_button_texture);

    music_button.setOrigin(music_button.getLocalBounds().width/2, music_button.getLocalBounds().height/2);
    sound_button.setOrigin(sound_button.getLocalBounds().width/2, sound_button.getLocalBounds().height/2);
    ok_button.setOrigin(ok_button.getLocalBounds().width/2, ok_button.getLocalBounds().height/2);
    
    if (!font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        std::cerr << "Failed to load font: /System/Library/Fonts/Helvetica.ttc" << std::endl;
        throw std::runtime_error("Failed to load required font");
    }
    
    start_text.setFont(font);
    start_text.setString("Start Game");
    start_text.setCharacterSize(30);
    start_text.setFillColor(sf::Color(228, 206, 83, 255));
    
    sf::FloatRect textRect = start_text.getLocalBounds();
    start_text.setOrigin(textRect.width/2, textRect.height/2);
    start_text.setPosition(GameConstants::WINDOW_WIDTH/2, GameConstants::WINDOW_HEIGHT/2);
    
    if (!menu_music.openFromFile("data/soundtracks/menu.ogg")) {
        std::cerr << "Ошибка при загрузке музыки меню" << std::endl;
    } else {
        menu_music.setLoop(true);
        menu_music.setVolume(50.f);
        menu_music.play();
    }
    
    settings_menu_active = false;
    settings_menu_position = 0.0f;
    is_music_on = true;
    is_sound_on = true;
}

MenuState::~MenuState() {
    menu_music.stop();
}

void MenuState::handle_input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        
        if (event.key.code == sf::Keyboard::Num2 && event.key.control) {
            game.change_state(new PlayState(game, "data/maps/map_new_2.bnd"));
            return;
        }
    }
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(game.get_window());
            sf::Vector2f worldPos = game.get_window().mapPixelToCoords(mouse_pos);
            
            if (settings_menu_active) {
                if (is_point_in_sprite(worldPos, music_button)) {
                    is_music_on = !is_music_on;
                    music_button.setTexture(is_music_on ? music_on_texture : music_off_texture);
                    game.set_music_enabled(is_music_on);
                    if (is_music_on) {
                        menu_music.play();
                    } else {
                        menu_music.pause();
                    }
                }
                else if (is_point_in_sprite(worldPos, sound_button)) {
                    is_sound_on = !is_sound_on;
                    sound_button.setTexture(is_sound_on ? sound_on_texture : sound_off_texture);
                    game.set_sound_enabled(is_sound_on);
                }
                else if (is_point_in_sprite(worldPos, ok_button)) {
                    settings_menu_active = false;
                }
                else if (!is_point_in_sprite(worldPos, settings_menu_sprite)) {
                    settings_menu_active = false;
                }
            }
            else {
                
                if (is_point_in_text(worldPos, start_text)) {
                    game.change_state(new LevelSelectState(game));
                }
                else if (is_point_in_sprite(worldPos, settings_sprite)) {
                    settings_menu_active = true;
                }
            }
        }
    }
}

void MenuState::update() {
    float target_position = settings_menu_active ? 1.0f : 0.0f;
    
    
    if (settings_menu_position != target_position) {
        if (settings_menu_active) {
            settings_menu_position = std::min(settings_menu_position + SETTINGS_ANIMATION_SPEED * 0.016f, 1.0f);
        } else {
            settings_menu_position = std::max(settings_menu_position - SETTINGS_ANIMATION_SPEED * 0.016f, 0.0f);
        }
        
        sf::FloatRect menuRect = settings_menu_sprite.getLocalBounds();
        float start_y = GameConstants::WINDOW_HEIGHT + menuRect.height;  
        float end_y = GameConstants::WINDOW_HEIGHT * 0.1f;              
        float current_y = start_y + (end_y - start_y) * settings_menu_position;
        
        settings_menu_sprite.setPosition(
            GameConstants::WINDOW_WIDTH/2,
            current_y
        );
        
        update_button_positions();
    }
}

void MenuState::update_button_positions() {
    sf::Vector2u window_size = game.get_window().getSize();
    sf::FloatRect menu_bounds = settings_menu_sprite.getGlobalBounds();
    float menu_center_y = settings_menu_sprite.getPosition().y + menu_bounds.height * 0.4f;
    
    
    sf::FloatRect music_bounds = music_button.getGlobalBounds();
    music_button.setPosition(
        window_size.x / 2.0f - 50.0f,
        menu_center_y
    );
    
    
    sf::FloatRect sound_bounds = sound_button.getGlobalBounds();
    sound_button.setPosition(
        window_size.x / 2.0f + 50.0f,
        menu_center_y
    );
    
    
    sf::FloatRect ok_bounds = ok_button.getGlobalBounds();
    ok_button.setPosition(
        window_size.x / 2.0f,
        menu_center_y + ok_bounds.height + 100.0f
    );
}

bool MenuState::is_point_in_sprite(const sf::Vector2f& point, const sf::Sprite& sprite) const {
    return sprite.getGlobalBounds().contains(point);
}

bool MenuState::is_point_in_text(const sf::Vector2f& point, const sf::Text& text) const {
    return text.getGlobalBounds().contains(point);
}

void MenuState::render(sf::RenderWindow& window) {
    window.draw(background_sprite);
    window.draw(logo_sprite);
    
    
    if (settings_menu_position > 0.0f) {
        window.draw(settings_menu_sprite);
        window.draw(music_button);
        window.draw(sound_button);
        window.draw(ok_button);
    }
    
    
    if (settings_menu_position < 1.0f) {
        window.draw(start_text);
        window.draw(settings_sprite);
    }
}
