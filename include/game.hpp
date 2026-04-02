#pragma once

#include <map>
#include <set>
#include <random>
#include <stdexcept>
#include <compare>

#include "graphics.hpp"

enum GameState
{
    LOST,
    WON,
    UNDECIDED
};

struct CellCoord
{
    unsigned int col;
    unsigned int row;

    auto operator<=>(const CellCoord &) const = default; // enables use in std::set/map
};

class GameGrid
{
public:
    GameGrid(unsigned int rows, unsigned int cols, unsigned int numBombs);
    bool toggleFlag(CellCoord coord);
    unsigned int numCols();
    unsigned int numRows();
    bool clickCell(CellCoord coord);
    GameState getGameState();
    std::set<CellCoord> bombPositions();
    std::map<CellCoord, unsigned int> revealedCells();

private:
    unsigned int cols;
    unsigned int rows;
    GameState gameState;
    std::map<CellCoord, unsigned char> numNeighbours;
    std::set<CellCoord> bombs;
    std::set<CellCoord> flags;
    std::map<CellCoord, unsigned int> revealed;
    std::set<CellCoord> generateBombs(unsigned int numBombs);
    void dfsReveal(CellCoord coord);
    std::set<CellCoord> neighbours4(CellCoord coord);
    std::set<CellCoord> neighbours8(CellCoord coord);
    unsigned int numBombNeighbours(CellCoord coord);
};

class GameDrawer {
    private:
        static std::vector<Texture> textures;

    public:
        static void drawGrid()
}