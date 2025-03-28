#include "Game.h"
#include "Wall.h"
#include "Tank.h"
#include "TextureManager.h"
#include "SDL_ttf.h"
using namespace std;
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), isMenu(true), backgroundMusic(nullptr) {}  // dấu hai chấm : là toán tử phạm vi dùng để xác định hàm Game() thuộc lớp Game.
Game::~Game() {} // đây là hàm hủy , ~ là toán tử hủy , dùng để giải phóng tài nguyên , tránh rò rỉ bộ nhớ.
// ta có thể hình dùng Game::init() và Game::update() nghĩa là hàm init() và update() của Game.\


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
		isRunning = false;
		return;
	}
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "Subsystems initialized!..." << endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
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
	player = Tank(6 * tile_size, 6 * tile_size, renderer);

	spawnEnemies();
	TextureManager::Instance()->load("D:/VISUAL STUDIO/BTLgame/Assets/menubackground.jpg", "menu_bg", renderer);
	TextureManager::Instance()->load("D:/VISUAL STUDIO/BTLgame/Assets/buttonplay.png", "button_play", renderer);
	if (!TextureManager::Instance()->load("D:/VISUAL STUDIO/BTLgame/Assets/bullet.png", "bullet", renderer)) {
		std::cout << "Failed to load bullet texture!" << std::endl;
		isRunning = false;
	}
	bulletTexture = TextureManager::Instance()->getTexture("bullet");
	if (!TextureManager::Instance()->getTexture("menu_bg") || !TextureManager::Instance()->getTexture("button_play")) {
		cout << "Failed to load menu assets!" << endl;
		isRunning = false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
		isRunning = false;
		return;
	}

	backgroundMusic = Mix_LoadMUS("D:/VISUAL STUDIO/BTLgame/Assets/backgroundMusic.wav");
	if (!backgroundMusic) {
		cout << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << endl;
		isRunning = false;
	}
	else {
		cout << "Background music loaded successfully!" << endl;
		// Chỉ phát nhạc nếu tải thành công
		if (Mix_PlayMusic(backgroundMusic, -1) == -1) {
			cout << "Failed to play background music: " << Mix_GetError() << endl;
		}
		else {
			cout << "Background music playing!" << endl;
		}
	}
	
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
			case SDLK_SPACE: player.shoot(player.bullets, bulletTexture);; break;
			}
		}
	}
}
void Game::update() {
	player.updateBullets();
	for (auto& enemy : enemies) {
		enemy.move(walls, player.x, player.y);
		enemy.updateBullets();
		if (enemy.bullets.size() < 3 && rand() % 100 < 2) {
			enemy.shoot(bulletTexture);
		}
	}

	// Xử lý va chạm giữa đạn và tường
	for (auto& wall : walls) {
		for (auto& bullet : player.bullets) {
			if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
				wall.active = false;
				bullet.active = false;
			}
		}
		for (auto& enemy : enemies) {
			for (auto& bullet : enemy.bullets) {
				if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
					wall.active = false;
					bullet.active = false;
				}
			}
		}
	}

	// Xử lý va chạm giữa đạn người chơi và kẻ địch
	for (auto& enemy : enemies) {
		for (auto& bullet : player.bullets) {
			if (bullet.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
				bullet.active = false;
				enemy.active = false;
			}
		}
	}

	// Xóa kẻ địch bị tiêu diệt
	enemies.erase(remove_if(enemies.begin(), enemies.end(), [](EnemyTank& e) { return !e.active; }), enemies.end());
	if (enemies.empty()) {
		isWin = true;  // Đánh dấu người chơi đã thắng
		gameOverTime = SDL_GetTicks();
		return;
	}

	// Kiểm tra va chạm giữa đạn kẻ địch và người chơi
	for (auto& enemy : enemies) {
		for (auto& bullet : enemy.bullets) {
			if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
				gameOver = true; // Đặt trạng thái game over
				gameOverTime = SDL_GetTicks();
				return; // Thoát để render xử lý
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
	SDL_Rect gameArea = { tile_size, tile_size, width - 2 * tile_size, height - 2 * tile_size };
	SDL_RenderFillRect(renderer, &gameArea);
	for (int i = 0; i < walls.size(); i++)
	{
		walls[i].render(renderer);
	}

	player.render(renderer);
	for (auto& bullet : player.bullets) {
		bullet.render(renderer);
	}
	for (auto& enemy : enemies)
	{
		enemy.render(renderer);
	}
	
	// Nếu gameOver, hiển thị "GAME OVER" và đợi 2 giây trước khi thoát
	if (gameOver) {
		renderGameOver("GAME OVER");
		SDL_Delay(3000); // Đợi 3 giây
		isRunning = false;
		return;
	}
	if (isWin) {
		renderGameOver("YOU WIN!");
		SDL_Delay(3000);
		isRunning = false;
		return;
	}


	SDL_RenderPresent(renderer);
}
void Game::clean()
{
	// Giải phóng nhạc nền
	Mix_FreeMusic(backgroundMusic);
	Mix_CloseAudio();
	Mix_Quit();
	SDL_DestroyTexture(bulletTexture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
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
			handleMenuEvents(event); // GỌI XỬ LÝ SỰ KIỆN MENU

			if (!isRunning) return; // Nếu chọn Quit thì thoát game ngay
			if (!isMenu) menuRunning = false; // Nếu chọn Play thì thoát menu
		}

		renderMenu(); // Cập nhật hiển thị menu
	}
}
void Game::renderMenu() {
	SDL_RenderClear(renderer);
	// Hiển thị background
	TextureManager::Instance()->draw("menu_bg", 0, 0, width, height, renderer);

	// Hiển thị button PLAY (kích thước thực: 213x42)
	SDL_Rect srcRectPlay = { 0, 0, 213, 42 };  // Kích thước thực của button_play.png
	SDL_Rect destRectPlay = { (width - 213) / 2, 250, 213, 42 };  // Căn giữa theo chiều ngang

	TextureManager::Instance()->draw("button_play",
		srcRectPlay.x, srcRectPlay.y, srcRectPlay.w, srcRectPlay.h,
		destRectPlay.x, destRectPlay.y, destRectPlay.w, destRectPlay.h,
		renderer);
	renderText(renderer, "Battle City", 50);
	renderText(renderer, "DO MINH NHAT I-IT6", 150);
	SDL_RenderPresent(renderer);
}
void Game::handleMenuEvents(SDL_Event& event) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		int mouseX = event.button.x;
		int mouseY = event.button.y;	

		// Kích thước vùng click khớp với kích thước hiển thị của button Play
		SDL_Rect playButtonDest = { (width - 213) / 2, 250, 213, 42 };  // Căn giữa theo chiều ngang

		if (mouseX >= playButtonDest.x && mouseX <= playButtonDest.x + playButtonDest.w &&
			mouseY >= playButtonDest.y && mouseY <= playButtonDest.y + playButtonDest.h) {
			// Chuyển trạng thái game sang GAME_RUNNING
			isMenu = false;
			gameState = GAME_RUNNING;
		}
	}
}

void Game::renderText(SDL_Renderer* renderer, const std::string& text, int y) {
	TTF_Font* font = TTF_OpenFont("D:/VISUAL STUDIO/BTLgame/Assets/game.ttf", 95);
	if (!font) {
		std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ để báo lỗi
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		return;
	}

	SDL_Color textColor = { 255, 255, 255, 255 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (!textSurface) {
		std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
		TTF_CloseFont(font);
		return;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!textTexture) {
		std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(textSurface);
		TTF_CloseFont(font);
		return;
	}

	// Lấy kích thước thực của chữ
	int textWidth = textSurface->w;
	int textHeight = textSurface->h;

	// Căn giữa theo chiều ngang
	SDL_Rect textRect = { (width - textWidth) / 2, y, textWidth, textHeight };
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	TTF_CloseFont(font);
}

void Game::renderGameOver(const std::string& message) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	renderText(renderer, message, height / 2 - 50);
	SDL_RenderPresent(renderer);
}