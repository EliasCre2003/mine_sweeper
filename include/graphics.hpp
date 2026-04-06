#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <utility>
#include <stdexcept>

class Texture
{
public:
    std::pair<int, int> drawSize;
    Texture(SDL_Surface *surface, const SDL_Rect sourceRect);
    // Texture fromPNG(std::string path);
    void draw(SDL_Surface *destSurface, int x, int y);
    // Texture fromSurface(SDL_Surface* surface);

private:
    SDL_Surface *surface;
    const SDL_Rect sourceRect;
};

class TextureAtlas
{
public:
    static TextureAtlas fromPNG(std::string path, std::pair<unsigned int, unsigned int> subdivision);
    Texture fetchTexture(unsigned int index);

private:
    std::pair<int, int> size;
    std::pair<unsigned int, unsigned int> subdivision;
    SDL_Surface *surface;
    TextureAtlas(SDL_Surface *surface, std::pair<unsigned int, unsigned int> subdivision);
};