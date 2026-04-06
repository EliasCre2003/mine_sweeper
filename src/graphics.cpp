#include "graphics.hpp"

TextureAtlas::TextureAtlas(
    SDL_Surface *surface,
    std::pair<unsigned int, unsigned int> subdivision) : surface(surface),
                                                         size({surface->w, surface->h}),
                                                         subdivision(subdivision) {}

TextureAtlas TextureAtlas::fromPNG(std::string path, std::pair<unsigned int, unsigned int> subdivision)
{
    SDL_Surface *textureSurface = SDL_LoadPNG(path.c_str());
    // SDL_Texture *texture;
    if (textureSurface == NULL)
    {
        throw std::invalid_argument("Could not load PNG");
    }
    // texture = SDL_CreateTextureFromSurface(textureSurface);
    // SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    // SDL_DestroySurface(textureSurface);

    // if (texture == NULL)
    // {
    //     throw std::invalid_argument("Could not create texture");
    // }

    return TextureAtlas(textureSurface, subdivision);
}

Texture TextureAtlas::fetchTexture(unsigned int index)
{
    int col = index % subdivision.first;
    int row = index / subdivision.first;
    int w = size.first / subdivision.first;
    int h = size.second / subdivision.second;
    SDL_Rect sourceRect = {
        w * col,
        h * row,
        w,
        h,
    };
    // sourceRect.w = size.first / subdivision.first;
    // sourceRect.h = size.second / subdivision.second;
    // sourceRect.x = sourceRect.w * col;
    // sourceRect.y = sourceRect.h * row;
    return Texture(surface, sourceRect);
}

Texture::Texture(SDL_Surface *surface, const SDL_Rect sourceRect) : surface(surface),
                                                                     sourceRect(sourceRect),
                                                                     drawSize(sourceRect.w, sourceRect.h) {}

void Texture::draw(SDL_Surface *destSurface, int x, int y)
{
    SDL_Rect destRect = {
        x,
        y,
        drawSize.first,
        drawSize.second};
    // SDL_RenderTexture(&sourceRect, &destRect);
    SDL_BlitSurfaceScaled(surface, &sourceRect, destSurface, &destRect, SDL_SCALEMODE_NEAREST);
}