#ifndef GAME_ENGINE_H_
#define GAME_ENGINE_H_

#include <SFML/Graphics.hpp>
#include "MenuState.h"

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    
    bool init();
    void handle_input();
    void update();
    void render();
    
    void change_state(GameState* new_state);
    sf::RenderWindow& get_window() { return window; }
    
    bool is_music_enabled() const { return music_enabled; }
    void set_music_enabled(bool enabled) { music_enabled = enabled; }
    
    bool is_sound_enabled() const { return sound_enabled; }
    void set_sound_enabled(bool enabled) { sound_enabled = enabled; }
    
    int get_max_level() const { return max_level; }
    void set_max_level(int level) { max_level = std::max(max_level, level); }
    
private:
    sf::RenderWindow window;
    GameState* current_state;
    bool music_enabled = true;
    bool sound_enabled = true;
    int max_level = 0;
};

#endif 
