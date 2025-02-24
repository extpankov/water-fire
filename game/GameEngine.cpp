#include "GameEngine.h"
#include "GameConstants.h"
#include <iostream>
#include <fstream>

GameEngine::GameEngine() : window(sf::VideoMode(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT), 
                                "Water & Fire Game", sf::Style::Close),
                          current_state(nullptr),
                          max_level(0) {
    std::cout << "Начало игры" << std::endl;
    
    std::ifstream progress_file("progress.txt");
    if (progress_file.is_open()) {
        progress_file >> max_level;
        progress_file.close();
        std::cout << "Loaded progress: max_level = " << max_level << std::endl;
    }
}

GameEngine::~GameEngine() {
    std::ofstream progress_file("progress.txt");
    if (progress_file.is_open()) {
        progress_file << max_level;
        progress_file.close();
        std::cout << "Saved progress: max_level = " << max_level << std::endl;
    }
    
    if (current_state) {
        delete current_state;
    }
}

bool GameEngine::init() {
    try {
        std::cout << "Создание окна..." << std::endl;
        
        
        window.create(sf::VideoMode(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT), "Fire and Water");
        if (!window.isOpen()) {
            std::cerr << "Ошибка при создании окна" << std::endl;
            return false;
        }
        window.setFramerateLimit(60);
        window.setVerticalSyncEnabled(true); 
        
        std::cout << "Установка начального состояния игры..." << std::endl;
        try {
            current_state = new MenuState(*this);
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка при создании начального состояния: " << e.what() << std::endl;
            return false;
        }
        
        if (!current_state) {
            std::cerr << "Начальное состояние не создано" << std::endl;
            return false;
        }
        
        std::cout << "Начальное состояние установлено успешно" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при инициализации: " << e.what() << std::endl;
        return false;
    }
    catch (...) {
        std::cerr << "Неизвестная ошибка при инициализации" << std::endl;
        return false;
    }
}

void GameEngine::handle_input() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (current_state) {
            current_state->handle_input(event);
        }
    }
}

void GameEngine::update() {
    if (current_state) {
        current_state->update();
    }
}

void GameEngine::render() {
    if (!window.isOpen()) {
        std::cerr << "Окно не открыто" << std::endl;
        return;
    }
    
    window.clear(sf::Color::Black);
    
    if (current_state) {
        current_state->render(window);
    } 
    
    window.display();
}

void GameEngine::change_state(GameState* new_state) {
    std::cout << "Changing game state. Current max_level: " << max_level << std::endl;
    if (current_state) {
        delete current_state;
    }
    current_state = new_state;
}
