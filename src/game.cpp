#include "game.hpp"
#include <algorithm>

using namespace std;

GameGrid::GameGrid(unsigned int rows, unsigned int cols, unsigned int numBombs)
    : cols(cols),
      rows(rows),
      numBombs(numBombs)
{
    reset();
}

void GameGrid::reset()
{
    bombs.clear();
    gameState = UNDECIDED;
    firstIsPlayed = false;
    flags.clear();
    revealed.clear();
}

unsigned int GameGrid::numCols()
{
    return cols;
}

unsigned int GameGrid::numRows()
{
    return rows;
}

bool GameGrid::clickCell(CellCoord coord)
{
    if (flags.contains(coord))
    {
        return true;
    }
    if (revealed.contains(coord))
        return true;
    if (bombs.contains(coord))
    {
        gameState = LOST;
        return false;
    }
    if (!firstIsPlayed)
    {
        bombs = generateBombs(numBombs, coord);
        firstIsPlayed = true;
    }
    dfsReveal(coord);
    if ((revealed.size() + bombs.size()) == (cols * rows))
        gameState = WON;
    return true;
}

bool GameGrid::toggleFlag(CellCoord coord)
{
    if (revealed.contains(coord))
    {
        return false;
    }
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

GameState GameGrid::getGameState()
{
    return gameState;
}

set<CellCoord> GameGrid::bombPositions()
{
    return bombs;
}

map<CellCoord, unsigned int> GameGrid::revealedCells()
{
    return revealed;
}

set<CellCoord> GameGrid::flagPositions()
{
    return flags;
}

set<CellCoord> GameGrid::generateBombs(unsigned int numBombs, CellCoord avoid)
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
    set<CellCoord> avoids = neighbours8(avoid);
    avoids.insert(avoid);
    while (bombs.size() < numBombs)
    {
        CellCoord coord = {
            colDist(gen),
            rowDist(gen)};
        if (!avoids.contains(coord))
            bombs.insert(coord);
    }

    return bombs;
}

set<CellCoord> GameGrid::neighbours4(CellCoord coord)
{
    set<CellCoord> neighbours;
    if (coord.col > 0)
        neighbours.insert({coord.col - 1, coord.row});
    if (coord.row > 0)
        neighbours.insert({coord.col, coord.row - 1});
    if (coord.col < (cols - 1))
        neighbours.insert({coord.col + 1, coord.row});
    if (coord.row < (rows - 1))
        neighbours.insert({coord.col, coord.row + 1});
    return neighbours;
}

set<CellCoord> GameGrid::neighbours8(CellCoord coord)
{
    set<CellCoord> neighbours = neighbours4(coord);
    if (coord.col > 0)
    {
        if (coord.row < (rows - 1))
            neighbours.insert({coord.col - 1, coord.row + 1});
        if (coord.row > 0)
            neighbours.insert({coord.col - 1, coord.row - 1});
    }
    if (coord.col < (cols - 1))
    {
        if (coord.row < (rows - 1))
            neighbours.insert({coord.col + 1, coord.row + 1});
        if (coord.row > 0)
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
    for (const auto &n : neighbours8(coord))
        dfsReveal(n);
}

unsigned int GameGrid::numBombNeighbours(CellCoord coord)
{
    unsigned int count = 0;
    for (const auto &n : neighbours8(coord))
        if (bombs.contains(n))
            count++;
    return count;
}

GameDrawer::GameDrawer(
    GameGrid &grid,
    std::pair<float, float> positonOffset,
    float cellSize) : grid(grid),
                      atlas(TextureAtlas::fromPNG(
                          "assets/minesweeper_texture_atlas.png",
                          std::pair<unsigned int, unsigned int>(4, 4))),
                      positionOffset(positonOffset),
                      cellSize(cellSize)
{
}

void GameDrawer::drawGrid(SDL_Surface *surface)
{
    map<CellCoord, unsigned int> revealed = grid.revealedCells();
    set<CellCoord> flags = grid.flagPositions();
    set<CellCoord> bombs = grid.bombPositions();
    for (unsigned int w = 0; w < grid.numCols(); w++)
    {
        for (unsigned int h = 0; h < grid.numRows(); h++)
        {
            CellCoord coord = {w, h};
            unsigned int textureIndex;
            if (flags.contains(coord))
            {
                textureIndex = 10;
            }
            else if ((grid.getGameState() == LOST) && bombs.contains(coord))
            {
                textureIndex = 12;
            }
            else if (revealed.contains(coord))
            {
                textureIndex = revealed.at(coord);
            }
            else
            {
                textureIndex = 9;
            }
            Texture texture = atlas.fetchTexture(textureIndex);
            texture.drawSize = {cellSize, cellSize};
            texture.draw(surface, positionOffset.first + cellSize * w, positionOffset.second + cellSize * h);
        }
    }
}

void GameDrawer::setGridPosition(std::pair<float, float> position)
{
    positionOffset = position;
}
void GameDrawer::setCellSize(float size)
{
    cellSize = size;
}