#include "GameEngine.h"
#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    try {
        std::cout << "Проверка текущей директории..." << std::endl;

        
        
        
        
        
        
        
        
        
        std::cout << "Создание игрового движка..." << std::endl;
        GameEngine game;
        
        if (!game.init()) {
            std::cerr << "Не удалось инициализировать игру!" << std::endl;
            return 1;
        }
        
        std::cout << "Начало игры..." << std::endl;
        
        while (game.get_window().isOpen()) {
            game.handle_input();
            game.update();
            game.render();
        }
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        std::cerr << "Тип ошибки: " << typeid(e).name() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Неизвестная ошибка произошла" << std::endl;
        return 1;
    }
}
