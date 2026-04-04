#include "bot.hpp"

RevealedCell::RevealedCell(Uint8 neighbours) : neighbours(neighbours) {}

Uint8 RevealedCell::hiddenBombs()
{
    return neighbours - foundBombs;
}

HiddenCell::HiddenCell(float bombProbability) : bombProbability(bombProbability), flagged(false) {}

Bot::Bot(GameGrid &gameGrid) : gameGrid(gameGrid)
{
    int cols = gameGrid.numCols(), rows = gameGrid.numCols();
    float probability = 1 / (cols * rows);
    for (int w = 0; w < cols; w++)
    {
        std::vector<std::unique_ptr<Cell>> row(rows);
        for (int r = 0; r < rows; r++)
        {
            row.push_back(std::make_unique<HiddenCell>(probability));
        }
        predictionGrid.push_back(row);
    }
};

void Bot::guessNext()
{
    for (int x = 0; x < gameGrid.numCols(); x++)
    {
        for (int y = 0; y < gameGrid.numRows(); y++)
        {
        }
    }
}

void Bot::updateProbabilities()
{
    for (int i = 0; i < 2; i++)
    {
        for (int x = 0; x < gameGrid.numCols(); x++)
        {
            for (int y = 0; y < gameGrid.numRows(); y++)
            {
                CellCoord coord = {x, y};
                Cell *cell = predictionGrid[x][y].get();
                if (i == 1)
                    checkHidden(coord);
                else
                    checkRevealed(coord);
            }
        }
    }
}

void Bot::checkHidden(CellCoord coord)
{
    Cell *cell = predictionGrid[coord.col][coord.row].get();
    HiddenCell *hCell = dynamic_cast<HiddenCell *>(cell);
    if (hCell == nullptr)
        return;
}

void Bot::checkRevealed(CellCoord coord)
{
    Cell *cell = predictionGrid[coord.col][coord.row].get();
    HiddenCell *rCell = dynamic_cast<HiddenCell *>(cell);
    if (rCell == nullptr)
        return;
    for (CellCoord n : gameGrid.neighbours8(coord)) 
    {
        
    }
}