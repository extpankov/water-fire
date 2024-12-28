#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <SFML/Graphics.hpp>

class GameState {
public:
    virtual ~GameState() = default;
    
    virtual void handle_input(const sf::Event& event) = 0;
    virtual void update() = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};

#endif // GAME_STATE_H_
