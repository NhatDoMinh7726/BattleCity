#pragma once
#include "Wall.h"
#include "SDL.h"
#include <vector>
#include "Kichthuoc.h"
#include "Bullet.h"
#include <algorithm>
#include "SDL_image.h"
using namespace std;
enum Direction { UP, RIGHT, DOWN, LEFT };
class Tank
{
public:
	Tank() : x(0), y(0), dirX(0), dirY(-1), rect{ 0, 0, tile_size, tile_size } {}
	~Tank() {};
	int x, y;
	int directionX = 0;
	int directionY = -1;
	SDL_Texture* tankTexture = nullptr;
	int dirX, dirY;
	SDL_Rect rect;
	Tank(int startX, int startY, SDL_Renderer* renderer) {
		x = startX;
		y = startY;
		rect = { x, y, tile_size, tile_size };
		direction = 0; // Mặc định hướng lên
		tankTexture = loadTexture("D:/VISUAL STUDIO/BTLgame/Assets/spritesheet.png", renderer);

		srcRect = { 0, 0, 64, 64 }; // Mặc định cắt hình xe tăng hướng lên
	}
	void move(int dx, int dy, const vector<Wall>& walls) {
		int newX = x + dx;
		int newY = y + dy;
		this->dirX = dx;
		this->dirY = dy;
		SDL_Rect newRect = { newX , newY , tile_size , tile_size };
		if (newX < tile_size || newX > width - tile_size * 2 || newY < tile_size || newY > height - tile_size * 2) {
			return; // Ngăn di chuyển ra ngoài màn hình
		}
		for (int i = 0; i < walls.size(); i++)
		{
			if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
				return; // Ngăn việc di chuyển nếu chạm vào tường 
			}
		}
		if (newX >= tile_size && newY <= width - tile_size * 2 && newY >= tile_size && newY <= height - tile_size * 2)
		{
			x = newX;
			y = newY;
			rect.x = x;
			rect.y = y;
		}
		if (dx > 0) direction = RIGHT;
		else if (dx < 0) direction = LEFT;
		else if (dy > 0) direction = DOWN;
		else if (dy < 0) direction = UP;
		if (dx != 0 || dy != 0) {
			directionX = dx;
			directionY = dy;
		}
		updateSprite(); // Cập nhật hình ảnh sau khi đổi hướng

	}
	vector<Bullet> bullets;
	void shoot(std::vector<Bullet>& bullets, SDL_Texture* bulletTexture) {
		bullets.push_back(Bullet(x, y, tile_size, dirX, dirY, bulletTexture));
	}
	void updateBullets() {
		for (auto& bullet : bullets) {
			bullet.move();
		}
		bullets.erase(remove_if(bullets.begin(), bullets.end(),
			[](Bullet& b) { return !b.active; }), bullets.end());
	}
	void render(SDL_Renderer* renderer) {
		if (tankTexture) {
			SDL_RenderCopy(renderer, tankTexture, &srcRect, &rect);
		}
		else {
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface) {
			std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
			return nullptr;
		}
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		return texture;
	}
	void updateSprite() {
		switch (direction) {
		case UP:
			srcRect.x = 192; // Hướng lên
			break;
		case RIGHT:
			srcRect.x = 128; // Hướng phải
			break;
		case DOWN:
			srcRect.x = 0;   // Hướng xuống
			break;
		case LEFT:
			srcRect.x = 64;  // Hướng trái
			break;
		}
	}
	void handleEvent(SDL_Event& e) {
		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_w: direction = 0; y -= tile_size; break; // Đi lên
			case SDLK_d: direction = 1; x += tile_size; break; // Đi phải
			case SDLK_s: direction = 2; y += tile_size; break; // Đi xuống
			case SDLK_a: direction = 3; x -= tile_size; break; // Đi trái
			}
			updateSprite(); // Cập nhật hình ảnh theo hướng
			rect.x = x;
			rect.y = y;
		}
	}
private:
	SDL_Rect srcRect; // Hình chữ nhật cắt từ spritesheet
	int direction; // 0: Lên, 1: Phải, 2: Xuống, 3: Trái

};
























