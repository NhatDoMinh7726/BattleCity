#pragma once
#include "Wall.h"
#include "SDL.h"
#include <vector>
#include "Kichthuoc.h"
#include "Bullet.h"
#include <algorithm>
using namespace std;

class Tank
{
public:
	Tank() : x(0), y(0), dirX(0), dirY(-1), rect{ 0, 0, tile_size, tile_size } {}
	~Tank() {};
	int x, y;
	int dirX, dirY;
	SDL_Rect rect;
	Tank(int startX, int startY)
	{
		x = startX;
		y = startY;
		rect = { x , y , tile_size , tile_size };
		dirX = 0;
		dirY = -1;
	}
	void move(int dx, int dy, const vector<Wall>& walls) {
		int newX = x + dx;
		int newY = y + dy;
		this->dirX = dx;
		this->dirY = dy;
		SDL_Rect newRect = { newX , newY , tile_size , tile_size };
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

	}
	vector<Bullet> bullets;
	void shoot() {
		bullets.push_back(Bullet(x + tile_size / 2 - 5, y + tile_size / 2 - 5, this->dirX, this->dirY));
	}
	void updateBullets() {
		for (auto& bullet : bullets)
		{
			bullet.move();
		}
		bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) {return !b.active; }), bullets.end());

	}
	void render(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
		for (auto& bullet : bullets)
		{
			bullet.render(renderer);
		}
	}
};

