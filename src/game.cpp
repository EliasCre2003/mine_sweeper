#include "game.hpp"
using namespace std;

GameGrid::GameGrid(unsigned int rows, unsigned int cols, unsigned int numBombs)
    : cols(cols),
      rows(rows),
      bombs(generateBombs(numBombs))
{
}

unsigned int GameGrid::numCols()
{
    return cols;
}

unsigned int GameGrid::numRows()
{
    return rows;
}

bool GameGrid::toggleFlag(unsigned int row, unsigned int col)
{
    CellCoord coord = {col, row};
    if (flags.contains(coord))
    {
        flags.erase(coord);
        return false;
    }
    else
    {
        flags.insert(coord);
        return true;
    }
}

set<CellCoord> GameGrid::generateBombs(unsigned int numBombs)
{
    if (numBombs > (cols * rows))
    {
        throw invalid_argument("Error: Too many bombs for the grid size!");
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<unsigned int> rowDist(0, rows - 1);
    uniform_int_distribution<unsigned int> colDist(0, cols - 1);

    set<CellCoord> bombs;
    while (bombs.size() < numBombs)
    {
        bombs.insert({colDist(gen),
                      rowDist(gen)});
    }

    return bombs;
}
