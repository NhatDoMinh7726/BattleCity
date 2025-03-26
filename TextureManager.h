#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <string>

using namespace std;

class TextureManager {
public:
    static TextureManager* Instance();

    bool load(const string& fileName, const string& id, SDL_Renderer* renderer);
    void draw(const string& id, int x, int y, int width, int height, SDL_Renderer* renderer);
    void draw(const string& id, int srcX, int srcY, int srcW, int srcH,
        int x, int y, int width, int height, SDL_Renderer* renderer);
    void drawFrame(const string& id, int x, int y, int width, int height, int row, int frame, SDL_Renderer* renderer);
    void clean();
    SDL_Texture* getTexture(const string& id);

private:
    TextureManager() = default;
    static TextureManager* s_Instance;
    map<string, SDL_Texture*> m_textureMap;
};
