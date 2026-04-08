#pragma once

#include <SDL3/SDL.h>
#include <string>

class Sound
{
public:
    Sound(std::string string);
    void play();

private:
    SDL_AudioSpec spec;
    Uint8 *wavData;
    Uint32 wavDataLen;
};