#include "LevelSelectState.h"
#include "GameEngine.h"
#include "MenuState.h"
#include "PlayState.h"
#include "GameConstants.h"
#include <iostream>

LevelSelectState::LevelSelectState(GameEngine& game) : game(game) {
    std::cout << "Инициализация состояния выбора уровней" << std::endl;
    std::cout << "Загрузка ресурсов выбора уровня..." << std::endl;
    
    if (!background_texture.loadFromFile("data/chose_level_bg.png")) {
        std::cerr << "Ошибка при загрузке фона выбора уровня: data/chose_level_bg.png" << std::endl;
        sf::Image bg;
        bg.create(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT, sf::Color(30, 30, 30));
        background_texture.loadFromImage(bg);
    }
    background_sprite.setTexture(background_texture);
    
    float scaleX = static_cast<float>(GameConstants::WINDOW_WIDTH) / background_texture.getSize().x;
    float scaleY = static_cast<float>(GameConstants::WINDOW_HEIGHT) / background_texture.getSize().y;
    background_sprite.setScale(scaleX, scaleY);
    
    if (!back_button_texture.loadFromFile("data/back_button.png")) {
        std::cerr << "Ошибка при загрузке кнопки назад: data/back_button.png" << std::endl;
        back_button.setTexture(sf::Texture());
        back_button.setColor(sf::Color(100, 100, 100));
    }
    back_button.setTexture(back_button_texture);
    
    back_button.setPosition(20, GameConstants::WINDOW_HEIGHT - back_button_texture.getSize().y - 20);
    
    fade_rect.setSize(sf::Vector2f(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT));
    fade_rect.setFillColor(sf::Color(0, 0, 0, 255));
    is_fading_in = true;
    is_fading_out = false;
    fade_alpha = 255.0f;

    load_progress_image();
}

void LevelSelectState::load_progress_image() {
    std::string filename = "data/step_" + std::to_string(max_unlocked_level) + ".png";
    std::cout << "Загрузка прогресса изображения: " << filename << std::endl;
    
    if (!level_progress_texture.loadFromFile(filename)) {
        std::cerr << "Не удалось загрузить прогресс изображения: " << filename << std::endl;
        return;
    }
    
    level_progress_sprite.setTexture(level_progress_texture);
    
    sf::FloatRect bounds = level_progress_sprite.getLocalBounds();
    level_progress_sprite.setOrigin(bounds.width/2, bounds.height/2);
    level_progress_sprite.setPosition(GameConstants::WINDOW_WIDTH/2, GameConstants::WINDOW_HEIGHT/2);
}

bool LevelSelectState::is_point_in_sprite(const sf::Vector2f& point, const sf::Sprite& sprite) {
    return sprite.getGlobalBounds().contains(point);
}

void LevelSelectState::start_fade_out() {
    is_fading_out = true;
    is_fading_in = false;
}

void LevelSelectState::handle_input(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(game.get_window());
            sf::Vector2f worldPos = game.get_window().mapPixelToCoords(mousePos);
            
            if (is_point_in_sprite(worldPos, back_button)) {
                start_fade_out();
            }
            else if (is_point_in_sprite(worldPos, level_progress_sprite)) {
                start_fade_out();
            }
        }
    }
}

void LevelSelectState::update() {
    const float fade_amount = FADE_SPEED * 0.016f;
    
    if (is_fading_in) {
        fade_alpha = std::max(0.0f, fade_alpha - fade_amount);
        if (fade_alpha == 0.0f) {
            is_fading_in = false;
        }
    }
    else if (is_fading_out) {
        fade_alpha = std::min(255.0f, fade_alpha + fade_amount);
        if (fade_alpha == 255.0f) {
            is_fading_out = false;
            
            sf::Vector2i mousePos = sf::Mouse::getPosition(game.get_window());
            sf::Vector2f worldPos = game.get_window().mapPixelToCoords(mousePos);
            
            if (is_point_in_sprite(worldPos, back_button)) {
                game.change_state(new MenuState(game));
            } else if (is_point_in_sprite(worldPos, level_progress_sprite)) {
                game.change_state(new PlayState(game));
            }
        }
    }
    
    fade_rect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fade_alpha)));
}

void LevelSelectState::render(sf::RenderWindow& window) {
    window.draw(background_sprite);
    window.draw(level_progress_sprite);
    window.draw(back_button);
    window.draw(fade_rect);
}
