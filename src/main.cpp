#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_timer.h>

// #include "graphics.hpp"
#include "game.hpp"

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 900;

int main(int, char **)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL: %s", SDL_GetError());
        printf("Error");
        return 3;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!SDL_CreateWindowAndRenderer(
            nullptr,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            0,
            &window,
            &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating window and renderer: %s", SDL_GetError());
    }

    // SDL_Window *window = SDL_CreateWindow(
    //     "TEST",
    //     1000, 1000, 0);

    // // SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

    // if (window == NULL)
    // {
    //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating window: %s", SDL_GetError());
    //     printf("Error");
    //     return 3;
    // }

    // SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    // if (renderer == NULL)
    // {
    //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating renderer: %s", SDL_GetError());
    //     printf("Error");
    //     return 3;
    // }

    // SDL_Surface* textureSurface = SDL_LoadPNG("assets/minesweeper_texture_atlas.png");
    // SDL_Texture* texture;
    // if (textureSurface != NULL) {
    //     texture = SDL_CreateTextureFromSurface(renderer, textureSurface);
    //     SDL_DestroySurface(textureSurface);
    // }

    // if (texture == NULL) {
    //     return 1;
    // }
    // Optional: Define a destination rectangle using floats (SDL_FRect)
    // If you pass NULL, it stretches to fill the whole window.
    // SDL_FRect destRect;
    // destRect.x = 100.0f;
    // destRect.y = 100.0f;
    // destRect.w = 200.0f;  // Width to draw
    // destRect.h = 200.0f;  // Height to draw

    // Draw the texture to the hidden backbuffer
    // Params: Renderer, Texture, Source Rect (NULL = whole image), Dest Rect

    // TextureAtlas atlas = TextureAtlas::fromPNG(
    //     renderer,
    //     "assets/minesweeper_texture_atlas.png",
    //     std::pair<unsigned int, unsigned int>(4, 4));
    // Texture texture = atlas.fetchTexture(1);
    // texture.drawSize = {100, 100};

    GameGrid grid(20, 20, 50);
    GameDrawer drawer(renderer, grid);
    float cellSize = 45;
    drawer.setCellSize(cellSize);

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        float x = 0, y = 0;
        SDL_MouseButtonFlags mouseFlag = SDL_GetMouseState(&x, &y);
        CellCoord coord = {
            (unsigned int) (y / cellSize),
            (unsigned int) (x / cellSize)
        };
        if (mouseFlag == SDL_BUTTON_LEFT)
        {
            grid.clickCell(coord);
        }

        // running = false;

        // SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);
        drawer.drawGrid();
        // texture.draw(renderer, 100, 100);
        // SDL_RenderTexture(renderer, texture, NULL, &destRect);
        // SDL_BlitSurface(textureAtlas, NULL, windowSurface, NULL);
        // SDL_UpdateWindowSurface(window);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}