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
    sf::RenderWindow& get_window();
    
    
    void set_music_enabled(bool enabled) { is_music_on = enabled; }
    void set_sound_enabled(bool enabled) { is_sound_on = enabled; }
    bool is_music_enabled() const { return is_music_on; }
    bool is_sound_enabled() const { return is_sound_on; }
    void toggle_music() { is_music_on = !is_music_on; }
    void toggle_sound() { is_sound_on = !is_sound_on; }
    
private:
    sf::RenderWindow window;
    GameState* current_state;
    bool is_music_on = true;
    bool is_sound_on = true;
};

#endif 
