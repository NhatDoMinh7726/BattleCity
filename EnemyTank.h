//#pragma once
//#include <vector>
//#include "SDL.h"
//#include<iostream>
//#include "Bullet.h"
//#include "Kichthuoc.h"
//#include "Wall.h"
//using namespace std;
//enum EnemyDirection { ENEMY_UP, ENEMY_RIGHT, ENEMY_DOWN, ENEMY_LEFT };
//class EnemyTank
//{
//public:
//    int x, y;
//    int dirX, dirY;
//    int moveDelay, shootDelay;
//    SDL_Rect rect;
//    bool active;
//    vector <Bullet> bullets;
//
//
//    EnemyTank(int startX, int startY)
//    {
//        moveDelay = 15;
//        shootDelay = 5;
//        x = startX;
//        y = startY;
//        rect = { x , y , tile_size , tile_size };
//        dirX = 0;
//        dirY = 1;
//        active = true;
//    }
//    
//
//    void move(const vector<Wall>& walls, int playerX, int playerY) {
//        if (--moveDelay > 0) return;
//        moveDelay = 10;
//
//        bool moved = false;
//        int attempts = 0;
//        const int maxAttempts = 4;
//
//        while (!moved && attempts < maxAttempts) {
//            int r = rand() % 6;
//            if (r < 4) { // 4/6 lần di chuyển ngẫu nhiên
//                if (r == 0) { dirX = 0; dirY = -5; }
//                else if (r == 1) { dirX = 0; dirY = 5; }
//                else if (r == 2) { dirX = -5; dirY = 0; }
//                else if (r == 3) { dirX = 5; dirY = 0; }
//            }
//            else { // 2/6 lần đuổi theo người chơi
//                // Ưu tiên hướng ngang hoặc dọc, không cho phép chéo
//                if (abs(playerX - x) > abs(playerY - y)) { // Ưu tiên hướng ngang
//                    dirX = (playerX > x) ? 5 : (playerX < x) ? -5 : 0;
//                    dirY = 0;
//                }
//                else { // Ưu tiên hướng dọc
//                    dirX = 0;
//                    dirY = (playerY > y) ? 5 : (playerY < y) ? -5 : 0;
//                }
//            }
//
//            int newX = x + dirX;
//            int newY = y + dirY;
//
//            if (newX < tile_size || newX > width - tile_size * 2 || newY < tile_size || newY > height - tile_size * 2) {
//                attempts++;
//                continue;
//            }
//
//            SDL_Rect newRect = { newX, newY, tile_size, tile_size };
//            bool collision = false;
//            for (const auto& wall : walls) {
//                if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
//                    collision = true;
//                    break;
//                }
//            }
//
//            if (!collision) {
//                x = newX;
//                y = newY;
//                rect.x = x;
//                rect.y = y;
//                moved = true;
//            }
//            attempts++;
//        }
//    }
//    void shoot() {
//        if (--shootDelay > 0) return;
//        shootDelay = 5;
//
//        // Kiểm tra nếu Enemy chưa di chuyển thì đặt hướng mặc định
//        if (dirX == 0 && dirY == 0) {
//            dirX = 0; dirY = 5; // Hướng mặc định: đi xuống
//        }
//
//        int bulletDirX = 0, bulletDirY = 0;
//        // Ưu tiên hướng ngang nếu cả hai hướng đều có
//        if (dirX != 0) {
//            bulletDirX = dirX;
//            bulletDirY = 0;
//        }
//        else if (dirY != 0) {
//            bulletDirX = 0;
//            bulletDirY = dirY;
//        }
//
//        bullets.push_back(Bullet(x + tile_size / 2 - 5, y + tile_size / 2 - 5, bulletDirX, bulletDirY));
//    }
//    void updateBullets() {
//        for (auto& bullet : bullets) {
//            bullet.move();
//        }
//        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.active; }), bullets.end());
//    }
//    void render(SDL_Renderer* renderer)
//    {
//        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//        SDL_RenderFillRect(renderer, &rect);
//        for (auto& bullet : bullets)
//        {
//            bullet.render(renderer);
//        }
//    }
//};
//
//
//
//
//
//
//
//


#pragma once
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "Bullet.h"
#include "Kichthuoc.h"
#include "Wall.h"
#include <iostream>
using namespace std;

enum EnemyDirection { ENEMY_UP, ENEMY_RIGHT, ENEMY_DOWN, ENEMY_LEFT };

class EnemyTank {
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    bool active;
    vector<Bullet> bullets;
    SDL_Texture* enemyTexture = nullptr; // Texture của enemy tank
    SDL_Rect srcRect = { 0, 0, 0, 0 };  // Cắt ảnh từ spritesheet
    int direction = ENEMY_DOWN;

    const int SPRITE_WIDTH = 194; // Kích thước 1 frame trong spritesheet
    const int SPRITE_HEIGHT = 237;
    const int SPRITES_PER_ROW = 4; // Số sprite trong một hàng

    EnemyTank(int startX, int startY, SDL_Renderer* renderer)
        : x(startX), y(startY), moveDelay(15), shootDelay(5),
        dirX(0), dirY(1), active(true), direction(ENEMY_DOWN) {

        rect = { x, y, tile_size, tile_size }; // Kích thước hiển thị = tile_size

        // Load texture
        enemyTexture = loadTexture("D:/VISUAL STUDIO/BTLgame/Assets/EnemyTank.png", renderer);

        if (enemyTexture == nullptr) {
            cout << "Failed to load EnemyTank texture!" << endl;
        }

        // Chọn sprite mặc định hướng xuống
        updateSprite();
    }

    ~EnemyTank() {
        if (enemyTexture) {
            SDL_DestroyTexture(enemyTexture);
        }
    }

    void move(const vector<Wall>& walls, int playerX, int playerY) {
        if (--moveDelay > 0) return;
        moveDelay = 10;

        bool moved = false;
        int attempts = 0;
        const int maxAttempts = 4;

        while (!moved && attempts < maxAttempts) {
            int r = rand() % 6;
            if (r < 4) {
                if (r == 0) { dirX = 0; dirY = -5; direction = ENEMY_UP; }
                else if (r == 1) { dirX = 0; dirY = 5; direction = ENEMY_DOWN; }
                else if (r == 2) { dirX = -5; dirY = 0; direction = ENEMY_LEFT; }
                else if (r == 3) { dirX = 5; dirY = 0; direction = ENEMY_RIGHT; }
            }
            else {
                if (abs(playerX - x) > abs(playerY - y)) {
                    dirX = (playerX > x) ? 5 : -5;
                    dirY = 0;
                    direction = (playerX > x) ? ENEMY_RIGHT : ENEMY_LEFT;
                }
                else {
                    dirX = 0;
                    dirY = (playerY > y) ? 5 : -5;
                    direction = (playerY > y) ? ENEMY_DOWN : ENEMY_UP;
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
                updateSprite();
                moved = true;
            }
            attempts++;
        }
    }

    void updateSprite() {
        if (enemyTexture) {
            srcRect = { direction * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT };
        }
    }

    void shoot() {
        if (--shootDelay > 0) return;
        shootDelay = 5;

        if (dirX == 0 && dirY == 0) {
            dirX = 0; dirY = 5;
        }

        int bulletDirX = (dirX != 0) ? dirX : 0;
        int bulletDirY = (dirY != 0) ? dirY : 0;

        bullets.push_back(Bullet(x + tile_size / 2 - 5, y + tile_size / 2 - 5, bulletDirX, bulletDirY));
    }

    void updateBullets() {
        for (auto& bullet : bullets) {
            bullet.move();
        }
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.active; }), bullets.end());
    }

    void render(SDL_Renderer* renderer) {
        if (!enemyTexture) return; // Nếu chưa load được ảnh thì không vẽ

        SDL_Rect dstRect = { x, y, tile_size, tile_size };
        SDL_RenderCopy(renderer, enemyTexture, &srcRect, &dstRect);

        for (auto& bullet : bullets) {
            bullet.render(renderer);
        }
    }

    SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
        SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
        if (!texture) {
            cout << "Failed to load texture: " << path << " Error: " << IMG_GetError() << endl;
        }
        return texture;
    }

    EnemyTank(const EnemyTank&) = delete;
    EnemyTank& operator=(const EnemyTank&) = delete;

    EnemyTank(EnemyTank&& other) noexcept {
        *this = std::move(other);
    }

    EnemyTank& operator=(EnemyTank&& other) noexcept {
        if (this != &other) {
            x = other.x;
            y = other.y;
            dirX = other.dirX;
            dirY = other.dirY;
            moveDelay = other.moveDelay;
            shootDelay = other.shootDelay;
            rect = other.rect;
            active = other.active;
            direction = other.direction;
            bullets = std::move(other.bullets);

            enemyTexture = other.enemyTexture;
            other.enemyTexture = nullptr;
        }
        return *this;
    }
};

