#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_timer.h>

#include "game.hpp"
#include "sounds.hpp"

const int nRows = 16;
const int nCols = 30;
const float cellSize = 46;
const int nBombs = 99;

int main(int, char **)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing SDL: %s", SDL_GetError());
        return 3;
    }

    Sound winSound("assets/sounds/win.wav");
    Sound loseSound("assets/sounds/lose.wav");
    Sound tickSound("assets/sounds/tick.wav");

    GameGrid grid(nRows, nCols, nBombs);

    const int SCREEN_WIDTH = nCols * cellSize;
    const int SCREEN_HEIGHT = nRows * cellSize;

    SDL_Window *window = SDL_CreateWindow(
        "Minesweeper",
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating window and renderer: %s", SDL_GetError());
    }

    SDL_SetWindowSurfaceVSync(window, 1);

    GameDrawer drawer(grid);
    drawer.setCellSize(cellSize);

    auto updateScreen = [&]()
    {
        SDL_Surface *screen = SDL_GetWindowSurface(window);
        if (!screen)
            return;

        const Uint32 bg = SDL_MapSurfaceRGBA(screen, 100, 100, 100, 255);
        SDL_FillSurfaceRect(screen, nullptr, bg);

        drawer.drawGrid(screen); // changed signature (see below)

        SDL_UpdateWindowSurface(window);
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
                    (unsigned int)(x / cellSize),
                    (unsigned int)(y / cellSize)};
                if (mouseFlag & SDL_BUTTON_LMASK)
                {
                    grid.clickCell(coord);
                    switch (grid.getGameState())
                    {
                    case WON:
                        winSound.play();
                    case LOST:
                        loseSound.play();
                    }
                }
                else if (mouseFlag & SDL_BUTTON_RMASK)
                {
                    grid.toggleFlag(coord);
                    tickSound.play();
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

    // SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}