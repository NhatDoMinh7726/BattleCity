#include "Game.h"
#include <iostream>
#include "Wall.h"
#include "Tank.h"
#include "Bullet.h"
#include "EnemyTank.h"

Game* game = nullptr; // game là con trỏ của lớp Game

int main(int argc, char* argv[])
{
    game = new Game();
    game->init("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

    // Vòng lặp menu
    while (game->getGameState() == GAME_MENU) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            game->handleMenuEvents(event); // Xử lý sự kiện menu
        }
        game->renderMenu(); // Hiển thị menu
        SDL_Delay(16);
    }

    // Nếu nhấn PLAY, bắt đầu game
    while (game->running()) {
        game->handleEvents();
        game->update();
        game->render();
        SDL_Delay(16);
    }

    game->clean();
    return 0;
}