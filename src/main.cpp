#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_timer.h>

#include "game.hpp"

const int nRows = 16;
const int nCols = 16;
const float cellSize = 48;
const int nBombs = 40;

int main(int, char **)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL: %s", SDL_GetError());
        return 3;
    }

    GameGrid grid(nRows, nCols, nBombs);

    const int SCREEN_WIDTH = nCols * cellSize;
    const int SCREEN_HEIGHT = nRows * cellSize;

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

    SDL_SetRenderVSync(renderer, 1);

    GameDrawer drawer(renderer, grid);
    drawer.setCellSize(cellSize);

    auto updateScreen = [renderer, &drawer]()
    {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);
        drawer.drawGrid();
        SDL_RenderPresent(renderer);
    };
    updateScreen();

    bool running = true;
    while (running)
    {
        bool redraw = false;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if ((event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) &&
                (grid.getGameState() == UNDECIDED))
            {
                float x = 0, y = 0;
                SDL_MouseButtonFlags mouseFlag = SDL_GetMouseState(&x, &y);
                CellCoord coord = {
                    (unsigned int)(y / cellSize),
                    (unsigned int)(x / cellSize)};
                if (mouseFlag & SDL_BUTTON_LMASK)
                {
                    grid.clickCell(coord);
                }
                else if (mouseFlag & SDL_BUTTON_RMASK)
                {
                    grid.toggleFlag(coord);
                }
            }
            else if (event.type = SDL_EVENT_KEY_DOWN)
            {
                const bool *keyStates = SDL_GetKeyboardState(nullptr);
                if (keyStates[SDL_SCANCODE_R])
                {
                    grid.reset();
                }
            }
            else
            {
                continue;
            }
            redraw = true;
        }
        if (redraw)
        {
            updateScreen();
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}