#include <iostream>
#include <SDL3/SDL.h>

#include "game.hpp"
#include "sounds.hpp"

#define EASY 0
#define MEDIUM 1
#define HARD 2

const int DIFFICULTY = MEDIUM;
const int difficulties[3][3] = {
    {9, 9, 9},
    {16, 16, 40},
    {16, 30, 99}};

const int *difficuiltyPreset = difficulties[DIFFICULTY];

const int nRows = difficuiltyPreset[0], nCols = difficuiltyPreset[1], nBombs = difficuiltyPreset[2];

const float cellSize = 64;

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
        return 3;
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

        drawer.drawGrid(screen);

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
                float x, y;
                SDL_MouseButtonFlags mouseFlag = SDL_GetMouseState(&x, &y);
                CellCoord coord = {
                    (unsigned int)(x / cellSize),
                    (unsigned int)(y / cellSize)};
                if (mouseFlag & SDL_BUTTON_LMASK)
                {
                    grid.clickCell(coord);
                }
                else if (mouseFlag & SDL_BUTTON_RMASK)
                {
                    if (grid.toggleFlag(coord))
                    {
                        tickSound.play();
                    }
                }
                switch (grid.getGameState())
                {
                case WON:
                    winSound.play();
                    break;
                case LOST:
                    loseSound.play();
                    break;
                }
            }
            else if (event.type = SDL_EVENT_KEY_DOWN)
            {
                const bool *keyStates = SDL_GetKeyboardState(nullptr);
                if (keyStates[SDL_SCANCODE_ESCAPE])
                {
                    running = false;
                }
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

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}