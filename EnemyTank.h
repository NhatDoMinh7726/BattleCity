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
    //void move(const vector<Wall>& walls)
    //{
    //    if (--moveDelay > 0)
    //        return;
    //    moveDelay = 15;

    //    int r = rand() % 4;
    //    if (r == 0) { dirX = 0; dirY = -5; } // Đi lên
    //    else if (r == 1) { dirX = 0; dirY = 5; } // Đi xuống
    //    else if (r == 2) { dirX = -5; dirY = 0; } // Đi trái
    //    else if (r == 3) { dirX = 5; dirY = 0; } // Đi phải

    //    int newX = x + dirX;
    //    int newY = y + dirY;
    //    SDL_Rect newRect = { newX , newY , tile_size , tile_size };

    //    for (const auto& wall : walls)
    //    {
    //        if (SDL_HasIntersection(&newRect, &wall.rect))
    //        {
    //            return;  // Nếu chạm tường thì không di chuyển
    //        }
    //    }

    //    x = newX;
    //    y = newY;
    //    rect.x = x;
    //    rect.y = y;
    //}

    //void move(const vector<Wall>& walls, int playerX, int playerY) {
    //    if (--moveDelay > 0) return;
    //    moveDelay = 10;

    //    int r = rand() % 6; // Tăng xác suất để có sự kết hợp giữa ngẫu nhiên và đuổi theo
    //    if (r < 4) { // 4/6 lần di chuyển ngẫu nhiên
    //        if (r == 0) { dirX = 0; dirY = -5; }
    //        else if (r == 1) { dirX = 0; dirY = 5; }
    //        else if (r == 2) { dirX = -5; dirY = 0; }
    //        else if (r == 3) { dirX = 5; dirY = 0; }
    //    }
    //    else { // 2/6 lần đuổi theo người chơi
    //        dirX = (playerX > x) ? 5 : (playerX < x) ? -5 : 0;
    //        dirY = (playerY > y) ? 5 : (playerY < y) ? -5 : 0;
    //    }

    //    int newX = x + dirX;
    //    int newY = y + dirY;

    //    // Kiểm tra giới hạn màn hình
    //    if (newX < tile_size || newX > width - tile_size * 2 || newY < tile_size || newY > height - tile_size * 2) {
    //        return;
    //    }

    //    SDL_Rect newRect = { newX, newY, tile_size, tile_size };
    //    for (const auto& wall : walls) {
    //        if (SDL_HasIntersection(&newRect, &wall.rect)) {
    //            return;
    //        }
    //    }

    //    x = newX;
    //    y = newY;
    //    rect.x = x;
    //    rect.y = y;
    //}

    void move(const vector<Wall>& walls, int playerX, int playerY) {
        if (--moveDelay > 0) return;
        moveDelay = 10;

        bool moved = false;
        int attempts = 0;
        const int maxAttempts = 4;

        while (!moved && attempts < maxAttempts) {
            int r = rand() % 6;
            if (r < 4) { // 4/6 lần di chuyển ngẫu nhiên
                if (r == 0) { dirX = 0; dirY = -5; }
                else if (r == 1) { dirX = 0; dirY = 5; }
                else if (r == 2) { dirX = -5; dirY = 0; }
                else if (r == 3) { dirX = 5; dirY = 0; }
            }
            else { // 2/6 lần đuổi theo người chơi
                // Ưu tiên hướng ngang hoặc dọc, không cho phép chéo
                if (abs(playerX - x) > abs(playerY - y)) { // Ưu tiên hướng ngang
                    dirX = (playerX > x) ? 5 : (playerX < x) ? -5 : 0;
                    dirY = 0;
                }
                else { // Ưu tiên hướng dọc
                    dirX = 0;
                    dirY = (playerY > y) ? 5 : (playerY < y) ? -5 : 0;
                }
            }

            int newX = x + dirX;
            int newY = y + dirY;

            if (newX < tile_size || newX > width - tile_size * 2 || newY < tile_size || newY > height - tile_size * 2) {
                attempts++;
                continue;
            }

            SDL_Rect newRect = { newX, newY, tile_size, tile_size };
            bool collision = false;
            for (const auto& wall : walls) {
                if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                x = newX;
                y = newY;
                rect.x = x;
                rect.y = y;
                moved = true;
            }
            attempts++;
        }
    }
    void shoot() {
        if (--shootDelay > 0) return;
        shootDelay = 5;

        // Kiểm tra nếu Enemy chưa di chuyển thì đặt hướng mặc định
        if (dirX == 0 && dirY == 0) {
            dirX = 0; dirY = 5; // Hướng mặc định: đi xuống
        }

        int bulletDirX = 0, bulletDirY = 0;
        // Ưu tiên hướng ngang nếu cả hai hướng đều có
        if (dirX != 0) {
            bulletDirX = dirX;
            bulletDirY = 0;
        }
        else if (dirY != 0) {
            bulletDirX = 0;
            bulletDirY = dirY;
        }

        bullets.push_back(Bullet(x + tile_size / 2 - 5, y + tile_size / 2 - 5, bulletDirX, bulletDirY));
    }
    void updateBullets() {
        for (auto& bullet : bullets) {
            bullet.move();
        }
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.active; }), bullets.end());
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









