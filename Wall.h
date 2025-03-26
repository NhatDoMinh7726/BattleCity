#pragma once
#include"SDL.h"
#include <iostream>
#include"Kichthuoc.h"
class Wall {
public:
    int x, y;
    SDL_Rect rect;
    bool active;

    Wall(int startX, int startY);
    void render(SDL_Renderer* renderer);
};


