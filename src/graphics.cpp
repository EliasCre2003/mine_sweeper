#include "graphics.hpp"

TextureAtlas::TextureAtlas(
    SDL_Texture *texture,
    std::pair<unsigned int, unsigned int> subdivision) : texture(texture),
                                                         size({texture->w, texture->h}),
                                                         subdivision(subdivision) {}

TextureAtlas TextureAtlas::fromPNG(SDL_Renderer *renderer, std::string path, std::pair<unsigned int, unsigned int> subdivision)
{
    SDL_Surface *textureSurface = SDL_LoadPNG(path.c_str());
    SDL_Texture *texture;
    if (textureSurface == NULL)
    {
        throw std::invalid_argument("Could not load PNG");
    }
    texture = SDL_CreateTextureFromSurface(renderer, textureSurface);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_DestroySurface(textureSurface);

    if (texture == NULL)
    {
        throw std::invalid_argument("Could not create texture");
    }

    return TextureAtlas(texture, subdivision);
}

Texture TextureAtlas::fetchTexture(unsigned int row, unsigned int col)
{
    SDL_FRect sourceRect;
    sourceRect.w = size.first / subdivision.first;
    sourceRect.h = size.second / subdivision.second;
    sourceRect.x = sourceRect.w * col;
    sourceRect.y = sourceRect.h * row;
    return Texture(texture, sourceRect);
}

Texture::Texture(SDL_Texture *texture, SDL_FRect sourceRect) : texture(texture),
                                                                sourceRect(sourceRect),
                                                                drawSize(sourceRect.w, sourceRect.h) {}

void Texture::draw(SDL_Renderer *renderer, float x, float y)
{
    SDL_FRect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = drawSize.first;
    destRect.h = drawSize.second;
    SDL_RenderTexture(renderer, texture, &sourceRect, &destRect);
}