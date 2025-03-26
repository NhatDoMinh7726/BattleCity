#pragma once
#include"SDL.h"
#include <vector>
#include"Wall.h"
#include <iostream>
#include "Kichthuoc.h"
#include "Tank.h"
#include "Bullet.h"
#include "EnemyTank.h"
using namespace std;
class Game {
public:
	Game(); 
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() {
		return isRunning;
	}	
	vector<Wall> walls; 
	void generateWalls()
	{
		for (int i = 3; i < map_height - 3; i += 2)
		{
			for (int j = 3; j < map_width - 3; j += 2)
			{
				Wall w = Wall(j * tile_size, i * tile_size);
				walls.push_back(w);
			}
		}
	}
	Tank player; 
	int enemyNumber = 3;
	vector<EnemyTank> enemies;
	void spawnEnemies() {
		enemies.clear();
		for (int i = 0; i < enemyNumber; ++i)
		{
			int ex, ey;
			bool validPosition = false;
			while (!validPosition)
			{
				ex = (rand() % (map_width - 2) + 1) * tile_size;
				ey = (rand() % (map_height - 2) + 1) * tile_size;
				validPosition = true;
				for (const auto& wall : walls)
				{
					if (wall.active && wall.x == ex && wall.y == ey)
					{
						validPosition = false;
						break;
					}
				}
			}
			enemies.push_back(EnemyTank(ex, ey));
		}
	}

	bool isMenu = true;
	void showMenu();
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;



};