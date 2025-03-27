
#pragma once
#include"SDL.h"
#include"Kichthuoc.h"
class Bullet
{
public:
	int x, y;
	int dx, dy;
	SDL_Rect rect;
	bool active;
	Bullet(int startX, int startY, int dirX, int dirY)
	{
		x = startX;
		y = startY;
		dx = dirX;
		dy = dirY;
		active = true;
		rect = { x , y , 10 ,10 };
	}
	void move() {
		x += dx;
		y += dy;
		rect.x = x;
		rect.y = y;
		if (x < tile_size || x > width - tile_size || y < tile_size || y > height - tile_size)
		{
			active = false;
		}
	}
	void render(SDL_Renderer* renderer)
	{
		if (active)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
};


//#pragma once
//#include "SDL.h"
//#include "Kichthuoc.h"
//
//class Bullet {
//public:
//    Bullet(int x, int y, int dx, int dy) : dirX(dx), dirY(dy), active(true) {
//        rect = { x, y, 10, 10 };
//    }
//
//    void move() {
//        rect.x += dirX;
//        rect.y += dirY;
//        if (rect.x < 0 || rect.x > width || rect.y < 0 || rect.y > height) {
//            active = false;
//        }
//    }
//
//    void render(SDL_Renderer* renderer) const {
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//        SDL_RenderFillRect(renderer, &rect);
//    }
//
//    SDL_Rect rect;
//    int dirX, dirY;
//    bool active;
//};