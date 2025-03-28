#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Kichthuoc.h"

class Bullet {
public:
    int dx, dy;
    SDL_Rect rect;
    bool active;
    SDL_Texture* texture;  // Thêm texture để lưu ảnh

    //Bullet(int x, int y, int dirX, int dirY, SDL_Texture* tex)
    //    : dx(dirX), dy(dirY), active(true), texture(tex) {
    //    rect = { x, y, 8, 8 }; // Tăng kích thước đạn
    //}

    Bullet(int tankX, int tankY, int tankSize, int dirX, int dirY, SDL_Texture* tex)
        : dx(dirX), dy(dirY), active(true), texture(tex) {
        rect = { tankX + tankSize / 2 - 6, tankY + tankSize / 2 - 6, 12, 12 }; // Căn giữa đạn
        if (dirX > 0)      rect.x += tankSize / 2;  // Bắn phải
        else if (dirX < 0) rect.x -= tankSize / 2;  // Bắn trái
        if (dirY > 0)      rect.y += tankSize / 2;  // Bắn xuống
        else if (dirY < 0) rect.y -= tankSize / 2;  // Bắn lên
    }


    void move() {
        rect.x += dx;
        rect.y += dy;
        if (rect.x < 0 || rect.x > width || rect.y < 0 || rect.y > height) {
            active = false;
        }
    }

    void render(SDL_Renderer* renderer) {
        if (active) {
            SDL_RenderCopy(renderer, texture, NULL, &rect);
        }
    }
};
