#include "game.hpp"
#include <algorithm>

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

bool GameGrid::clickCell(unsigned int row, unsigned int col)
{
    CellCoord coord = {col, row};
    if (revealed.contains(coord))
        return true;
    // if (bombs.contains(coord))
    // {
    //     revealed.merge(bombs);
    //     return false;
    // }
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

set<CellCoord> GameGrid::neighbours4(CellCoord coord)
{
    set<CellCoord> neighbours;
    if (coord.col != 0)
        neighbours.insert({coord.col - 1, coord.row});
    if (coord.row != 0)
        neighbours.insert({coord.col, coord.row - 1});
    if (coord.col != (cols - 1))
        neighbours.insert({coord.col + 1, coord.row});
    if (coord.col != (rows - 1))
        neighbours.insert({coord.col, coord.row + 1});
    return neighbours;
}

set<CellCoord> GameGrid::neighbours8(CellCoord coord)
{
    set<CellCoord> neighbours = neighbours4(coord);
    if (coord.col != 0)
    {
        if (coord.row != rows - 1)
            neighbours.insert({coord.col - 1, coord.row + 1});
        if (coord.row != 0)
            neighbours.insert({coord.col - 1, coord.row - 1});
    }
    if (coord.col != cols - 1)
    {
        if (coord.row != rows - 1)
            neighbours.insert({coord.col + 1, coord.row + 1});
        if (coord.row != 0)
            neighbours.insert({coord.col + 1, coord.row - 1});
    }
    return neighbours;
}

void GameGrid::dfsReveal(CellCoord coord)
{
    if (revealed.contains(coord))
        return;
    unsigned int bombNeighbours = numBombNeighbours(coord);
    revealed.insert_or_assign(coord, bombNeighbours);
    if (bombNeighbours != 0)
        return;
    for (CellCoord neighbour : neighbours4(coord))
        dfsReveal(neighbour);
}

unsigned int GameGrid::numBombNeighbours(CellCoord coord)
{
    set<CellCoord> neighbours = neighbours8(coord);
    return set_intersection(neighbours, bombs).size()
}