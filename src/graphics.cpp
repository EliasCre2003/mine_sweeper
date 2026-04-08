#include "graphics.hpp"

TextureAtlas::TextureAtlas(
    SDL_Surface *surface,
    std::pair<unsigned int, unsigned int> subdivision) : surface(surface),
                                                         size({surface->w, surface->h}),
                                                         subdivision(subdivision) {}

TextureAtlas TextureAtlas::fromPNG(std::string path, std::pair<unsigned int, unsigned int> subdivision)
{
    SDL_Surface *textureSurface = SDL_LoadPNG(path.c_str());
    if (textureSurface == NULL)
    {
        throw std::invalid_argument("Could not load PNG");
    }
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
    SDL_BlitSurfaceScaled(surface, &sourceRect, destSurface, &destRect, SDL_SCALEMODE_NEAREST);
}