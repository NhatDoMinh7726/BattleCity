#include "TextureManager.h"
#include <iostream>

using namespace std;

TextureManager* TextureManager::s_Instance = nullptr;

TextureManager* TextureManager::Instance() {
    if (!s_Instance) {
        s_Instance = new TextureManager();
    }
    return s_Instance;
}

bool TextureManager::load(const string& fileName, const string& id, SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    if (!tempSurface) {
        cerr << "Failed to load image " << fileName << ": " << IMG_GetError() << endl;
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (texture) {
        m_textureMap[id] = texture;
        return true;
    }

    return false;
}

void TextureManager::draw(const string& id, int x, int y, int width, int height, SDL_Renderer* renderer) {
    if (m_textureMap.find(id) == m_textureMap.end()) {
        cerr << "Texture ID " << id << " not found!" << endl;
        return;
    }

    SDL_Rect srcRect = { 0, 0, width, height };
    SDL_Rect destRect = { x, y, width, height };
    SDL_RenderCopy(renderer, m_textureMap[id], &srcRect, &destRect);
}

void TextureManager::draw(const string& id, int srcX, int srcY, int srcW, int srcH,
    int x, int y, int width, int height, SDL_Renderer* renderer) {
    if (m_textureMap.find(id) == m_textureMap.end()) {
        cerr << "Texture ID " << id << " not found!" << endl;
        return;
    }

    SDL_Rect srcRect = { srcX, srcY, srcW, srcH };
    SDL_Rect destRect = { x, y, width, height };
    SDL_RenderCopy(renderer, m_textureMap[id], &srcRect, &destRect);
}

void TextureManager::drawFrame(const string& id, int x, int y, int width, int height, int row, int frame, SDL_Renderer* renderer) {
    if (m_textureMap.find(id) == m_textureMap.end()) {
        cerr << "Texture ID " << id << " not found!" << endl;
        return;
    }
}
SDL_Texture* TextureManager::getTexture(const string& id) {
    if (m_textureMap.find(id) != m_textureMap.end()) {
        return m_textureMap[id];
    }
    return nullptr;
}