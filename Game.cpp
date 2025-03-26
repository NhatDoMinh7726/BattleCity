#include "Game.h"
#include "Wall.h"
#include "Tank.h"
#include "TextureManager.h"
using namespace std; 
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), isMenu(true) , backgroundMusic(nullptr) {}  // dấu hai chấm : là toán tử phạm vi dùng để xác định hàm Game() thuộc lớp Game.
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
	TextureManager::Instance()->load("D:/VISUAL STUDIO/BTLgame/Assets/menubackground.jpg", "menu_bg", renderer);
	TextureManager::Instance()->load("D:/VISUAL STUDIO/BTLgame/Assets/buttonplay.png", "button_play", renderer);
	if (!TextureManager::Instance()->getTexture("menu_bg") || !TextureManager::Instance()->getTexture("button_play")) {
		cout << "Failed to load menu assets!" << endl;
		isRunning = false;
	}	

	// Khởi tạo SDL_mixer
	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_WAV) != (MIX_INIT_OGG | MIX_INIT_WAV)) {
		cout << "Failed to initialize SDL_mixer: " << Mix_GetError() << endl;
	}
	else {
		cout << "SDL_mixer initialized successfully!" << endl;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		cout << "Failed to open audio: " << Mix_GetError() << endl;
	}
	else {
		cout << "Audio opened successfully!" << endl;
	}

	// Tải nhạc nền (file WAV)
	backgroundMusic = Mix_LoadMUS("D:/VISUAL STUDIO/BTLgame/Assets/background_music.wav");
	if (!backgroundMusic) {
		cout << "Failed to load background music: " << Mix_GetError() << endl;
		// Kiểm tra xem file có tồn tại không
		FILE* file = fopen("D:/VISUAL STUDIO/BTLgame/Assets/background_music2.wav", "rb");
		if (!file) {
			cout << "File background_music2.wav does not exist or cannot be accessed!" << endl;
		}
		else {
			fclose(file);
			cout << "File background_music2.wav exists, but SDL_mixer failed to load it!" << endl;
		}
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
			case SDLK_SPACE: player.shoot(); break;
			}
		}
	}
}
void Game::update(){
	player.updateBullets();
	for (auto& enemy : enemies)
	{
		enemy.move(walls , player.x , player.y);
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
	// Giải phóng nhạc nền
	Mix_FreeMusic(backgroundMusic);
	Mix_CloseAudio();
	Mix_Quit();

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
			handleMenuEvents(event); // GỌI XỬ LÝ SỰ KIỆN MENU

			if (!isRunning) return; // Nếu chọn Quit thì thoát game ngay
			if (!isMenu) menuRunning = false; // Nếu chọn Play thì thoát menu
		}

		renderMenu(); // Cập nhật hiển thị menu
	}
}
//void Game::renderMenu() {
//	// Hiển thị background
//	TextureManager::Instance()->draw("menu_bg", 0, 0, width, height, renderer);
//
//	//// Hiển thị nút Play
//	//TextureManager::Instance()->draw("button_play", 300, 250, 128, 64, renderer);
//
//	//// Hiển thị nút Quit
//	//TextureManager::Instance()->draw("button_quit", 300, 350, 128, 64, renderer);
//
//	//SDL_RenderPresent(renderer);
//	// Hiển thị button PLAY
//	
//
//}

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