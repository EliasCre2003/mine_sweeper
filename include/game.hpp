#pragma once

#include <map>
#include <set>
#include <random>
#include <stdexcept>
#include <compare>

struct CellCoord
{
    unsigned int col;
    unsigned int row;

    auto operator<=>(const CellCoord&) const = default; // enables use in std::set/map
};

class GameGrid
{
public:
    
    GameGrid(unsigned int rows, unsigned int cols, unsigned int numBombs);
    bool toggleFlag(unsigned int row, unsigned int col);

    unsigned int numCols();
    unsigned int numRows();

private:
    unsigned int cols;
    unsigned int rows;
    std::map<CellCoord, unsigned char> numNeighbours;
    std::set<CellCoord> bombs;
    std::set<CellCoord> flags;

    std::set<CellCoord> generateBombs(unsigned int numBombs);
};