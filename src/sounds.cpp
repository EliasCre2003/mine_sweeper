#include "sounds.hpp"

Sound::Sound(std::string string)
{
    if (!SDL_LoadWAV(string.c_str(), &spec, &wavData, &wavDataLen))
    {
        SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
    }
}

void Sound::play()
{
    SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    if (!stream)
    {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
    }
    SDL_ResumeAudioStreamDevice(stream);
    if (SDL_GetAudioStreamQueued(stream) < (int)wavDataLen)
    {
        /* feed more data to the stream. It will queue at the end, and trickle out as the hardware needs more data. */
        SDL_PutAudioStreamData(stream, wavData, wavDataLen);
    }
}