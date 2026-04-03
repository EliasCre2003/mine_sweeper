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

bool GameGrid::clickCell(CellCoord coord)
{
    // if (gameState != UNDECIDED) {

    // }
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
    dfsReveal(coord);
    return true;
}

bool GameGrid::toggleFlag(CellCoord coord)
{
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
    for (const auto &n : neighbours4(coord))
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
    SDL_Renderer *renderer,
    GameGrid &grid,
    std::pair<float, float> positonOffset,
    float cellSize) : renderer(renderer),
                      grid(grid),
                      atlas(TextureAtlas::fromPNG(
                          renderer,
                          "assets/minesweeper_texture_atlas.png",
                          std::pair<unsigned int, unsigned int>(4, 4))),
                      positionOffset(positonOffset),
                      cellSize(cellSize)
{
}

void GameDrawer::drawGrid()
{
    // unsigned int cellSize = 24;
    // unsigned int offsetX = 100;
    // unsigned int offsetY = 100;
    map<CellCoord, unsigned int> revealed = grid.revealedCells();
    set<CellCoord> flags = grid.flagPositions();
    for (unsigned int w = 0; w < grid.numCols(); w++)
    {
        for (unsigned int h = 0; h < grid.numRows(); h++)
        {
            CellCoord coord = {h, w};
            unsigned int textureIndex;
            if (flags.contains(coord))
            {
                textureIndex = 10;
            }
            else if (revealed.contains(coord))
            {
                textureIndex = revealed.at(coord);

                // .draw(renderer, offsetX + cellSize * w, offsetY + cellSize * h);
            }
            else
            {
                textureIndex = 9;
            }
            Texture texture = atlas.fetchTexture(textureIndex);
            texture.drawSize = {cellSize, cellSize};
            texture.draw(renderer, positionOffset.first + cellSize * w, positionOffset.second + cellSize * h);
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