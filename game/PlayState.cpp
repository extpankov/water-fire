#include "PlayState.h"
#include "GameEngine.h"
#include "GameConstants.h"
#include <iostream>

PlayState::PlayState(GameEngine& game) 
    : game(game)
    , level(game)
    , fade_alpha(255.0f)
    , is_fading_in(true) {
    std::cout << "Initializing play state..." << std::endl;
    level.init("data/maps/map_new_1.bnd");
    
    game_view = game.get_window().getDefaultView();
    
    fade_rect.setSize(sf::Vector2f(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT));
    fade_rect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fade_alpha)));
    
    lastFrameTime = frameClock.restart();
}

PlayState::PlayState(GameEngine& game, const std::string& mapFile) 
    : game(game)
    , level(game)
    , fade_alpha(255.0f)
    , is_fading_in(true) {
    std::cout << "Initializing play state..." << std::endl;
    level.init(mapFile);
    
    game_view = game.get_window().getDefaultView();
    
    fade_rect.setSize(sf::Vector2f(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT));
    fade_rect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fade_alpha)));
    
    lastFrameTime = frameClock.restart();
}

PlayState::~PlayState() {
}

void PlayState::handle_input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            game.change_state(new MenuState(game));
        }
    }
    
    level.handleInput(event);
}

void PlayState::update() {
    float deltaTime = frameClock.restart().asSeconds();
    
    if (is_fading_in) {
        fade_alpha = std::max(0.0f, fade_alpha - FADE_SPEED * deltaTime);
        if (fade_alpha == 0.0f) {
            is_fading_in = false;
        }
        fade_rect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fade_alpha)));
    }
    
    level.update(deltaTime);
    
    update_view();
}

void PlayState::render(sf::RenderWindow& window) {
    window.setView(game_view);
    
    level.draw(window, sf::Vector2f(0, 0), false);
    
    window.setView(window.getDefaultView());
    
    if (fade_alpha > 0.0f) {
        window.draw(fade_rect);
    }
}

void PlayState::update_view() {
    game_view.setCenter(GameConstants::WINDOW_WIDTH / 2.0f, GameConstants::WINDOW_HEIGHT / 2.0f);
}
