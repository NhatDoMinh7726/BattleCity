#include "Wall.h"
#include <SDL.h>

Wall::Wall(int startX, int startY) {
    x = startX;
    y = startY;
    active = true;
    rect = { x, y, tile_size, tile_size };
}

void Wall::render(SDL_Renderer* renderer) {
    if (active) {
        SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Màu nâu
        SDL_RenderFillRect(renderer, &rect);
    }
}


