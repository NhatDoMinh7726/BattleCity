//#include"Game.h"
//#include <iostream>
//#include "Wall.h"
//#include "Tank.h"
//#include "Bullet.h"
//#include "EnemyTank.h"
//Game* game = nullptr; // game ở đây đang là con trỏ của lớp Game
//int main(int argc, char* argv[])
//{
//	game = new Game();
//	game->init("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
//	game->showMenu();
//	while (game->running()) { 
//		game->handleEvents();
//		game->update();
//		game->render();
//		SDL_Delay(16);
//		// dấu -> là toán tử truy cập thành viên thông qua con trỏ . Khi có một
//		// con trỏ trỏ đến đối tg của lớp , sử dụng -> để truy cập các thành viên .
//	}	// game -> running nghĩa là gọi hàm running() của đối tượng game .
//		// game->running() = (*game).running() nhưng cách đầu ngắn và đễ đọc hơn .
//
//	game->clean();
//
//	return 0;
//}

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
