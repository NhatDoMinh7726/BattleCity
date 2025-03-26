#include "Game.h"
#include "Wall.h"
#include "Tank.h"
using namespace std; 
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), isMenu(true) {}  // dấu hai chấm : là toán tử phạm vi dùng để xác định hàm Game() thuộc lớp Game.
Game::~Game(){} // đây là hàm hủy , ~ là toán tử hủy , dùng để giải phóng tài nguyên , tránh rò rỉ bộ nhớ.
// ta có thể hình dùng Game::init() và Game::update() nghĩa là hàm init() và update() của Game.\


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "Subsystems initialized!..." << endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height , flags);
		if (window)
		{
			cout << "Window created!" << endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{	
			SDL_RenderPresent(renderer);
			cout << "Renderer created!" << endl;
		}
		isRunning = true;
	}
	else {
		isRunning = false; 
	}
	generateWalls();
	player = Tank(6 * tile_size, 6 * tile_size);
	spawnEnemies();



}
void Game::handleEvents()
{	
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			isRunning = false;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym) {
			case SDLK_UP: player.move(0, -5, walls); break;
			case SDLK_DOWN: player.move(0, 5, walls); break;
			case SDLK_RIGHT: player.move(5, 0, walls); break;
			case SDLK_LEFT: player.move(-5, 0, walls); break;
			case SDLK_SPACE: player.shoot(); break;
			}
		}
	}
}
void Game::update(){
	player.updateBullets();
	for (auto& enemy : enemies)
	{
		enemy.move(walls);
		enemy.updateBullets();
		// Giới hạn số lượng đạn trên màn hình
		if (enemy.bullets.size() < 3 && rand() % 100 < 2) {
				enemy.shoot();
		}
		
	}
	for (auto& wall : walls)
	{
		for (auto& bullet : player.bullets)
		{
			if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect))
			{
				wall.active = false;
				bullet.active = false;
			}
		}
		for (auto& enemy : enemies)
		{
			for (auto& bullet : enemy.bullets)
			{
				if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect))
				{
					wall.active = false;
					bullet.active = false;
				}
			}
		}
		for (auto& enemy : enemies)
		{
			for (auto& bullet : player.bullets)
			{
				if (bullet.active && SDL_HasIntersection(&bullet.rect, &enemy.rect))
				{
					bullet.active = false; // Xóa đạn
					enemy.active = false; // Xóa enemy
				}
			}
		}

	}

	enemies.erase(remove_if(enemies.begin(), enemies.end(), [](EnemyTank& e) {return !e.active; }), enemies.end());
	if (enemies.empty()) {
		isRunning = false;
	}
	for (auto& enemy : enemies)
	{
		for (auto& bullet : enemy.bullets)
		{
			if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
				isRunning = false;
				return;	
			}
		}
	}
}
void Game::render()
{
	// Vẽ viền xám trước
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Màu xám
	SDL_RenderClear(renderer);

	// Vẽ phần nền bên trong (màu đen)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect gameArea = { tile_size, tile_size, width - 2 * tile_size, height - 2 * tile_size};
	SDL_RenderFillRect(renderer, &gameArea);
	for (int i = 0; i < walls.size(); i++)
	{
		walls[i].render(renderer);
	}

	player.render(renderer);

	for (auto& enemy : enemies)
	{
		enemy.render(renderer);
	}
	SDL_RenderPresent(renderer);
}
void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game cleaned!" << endl;
}
void Game::showMenu() {
	SDL_Event event;
	bool menuRunning = true;

	while (menuRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
				return;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int x = event.button.x, y = event.button.y;
				if (x >= 300 && x <= 500 && y >= 200 && y <= 250) { // Nút Start Game
					menuRunning = false;
					isMenu = false;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
		SDL_RenderClear(renderer);

		SDL_Rect startButton = { 300, 200, 200, 50 };
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &startButton);

		SDL_RenderPresent(renderer);
	}
}

