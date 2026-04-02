#include <SDL3/SDL.h>
#include <string>
#include <utility>
#include <stdexcept>

class Texture
{
public:
    std::pair<float, float> drawSize;
    Texture(SDL_Texture *texture, SDL_FRect sourceRect);
    Texture fromPNG(std::string path);
    void draw(SDL_Renderer* renderer, float x, float y);
    // Texture fromSurface(SDL_Surface* surface);

private:
    SDL_Texture *texture;
    SDL_FRect sourceRect;
};

class TextureAtlas
{
public:
    static TextureAtlas fromPNG(SDL_Renderer *renderer, std::string path, std::pair<unsigned int, unsigned int> subdivision);
    Texture fetchTexture(unsigned int index);

private:
    std::pair<float, float> size;
    std::pair<unsigned int, unsigned int> subdivision;
    SDL_Texture *texture;
    TextureAtlas(SDL_Texture *texture, std::pair<unsigned int, unsigned int> subdivision);
};