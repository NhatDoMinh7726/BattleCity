#pragma once
#include <vector>
#include "SDL.h"
#include<iostream>
#include "Bullet.h"
#include "Kichthuoc.h"
#include "Wall.h"
using namespace std;
class EnemyTank
{
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    bool active;
    vector <Bullet> bullets;


    EnemyTank(int startX, int startY)
    {
        moveDelay = 15;
        shootDelay = 5;
        x = startX;
        y = startY;
        rect = { x , y , tile_size , tile_size };
        dirX = 0;
        dirY = 1;
        active = true;
    }
    void move(const vector<Wall>& walls)
    {
        if (--moveDelay > 0)
            return;
        moveDelay = 15;

        int r = rand() % 4;
        if (r == 0) { dirX = 0; dirY = -5; } // Đi lên
        else if (r == 1) { dirX = 0; dirY = 5; } // Đi xuống
        else if (r == 2) { dirX = -5; dirY = 0; } // Đi trái
        else if (r == 3) { dirX = 5; dirY = 0; } // Đi phải

        int newX = x + dirX;
        int newY = y + dirY;
        SDL_Rect newRect = { newX , newY , tile_size , tile_size };

        for (const auto& wall : walls)
        {
            if (SDL_HasIntersection(&newRect, &wall.rect))
            {
                return;  // Nếu chạm tường thì không di chuyển
            }
        }

        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }

    void shoot() {
    if (--shootDelay > 0) return;
    shootDelay = 5;

    // Kiểm tra nếu Enemy chưa di chuyển thì đặt hướng mặc định
    if (dirX == 0 && dirY == 0) {
        dirX = 0; dirY = 5;
    }

    bullets.push_back(Bullet(x + tile_size / 2 - 5, y + tile_size / 2 - 5, dirX, dirY));
}

    void updateBullets() {
        for (auto& bullet : bullets)
        {
            bullet.move();
        }
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) {return!b.active; }), bullets.end());

    }
    void render(SDL_Renderer* renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        for (auto& bullet : bullets)
        {
            bullet.render(renderer);
        }
    }
};
