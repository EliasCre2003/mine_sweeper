#pragma once

#include <random>
#include <memory>

#include "game.hpp"


class Cell
{
    virtual ~Cell() = default;
};

class RevealedCell : public Cell
{
public:
    Uint8 foundBombs;
    RevealedCell(Uint8 neighbours);
    Uint8 hiddenBombs();

private:
    Uint8 neighbours;
};

class HiddenCell : public Cell
{
public:
    HiddenCell(float bombProbability = 0.0F);
    float bombProbability;
    bool flagged;
};

class Bot
{
public:
    Bot(GameGrid &gameGrid);
    void guessNext();

private:
    GameGrid &gameGrid;
    std::vector<std::vector<std::unique_ptr<Cell>>> predictionGrid;

    void updateProbabilities();
    void checkHidden(CellCoord coord);
    void checkRevealed(CellCoord coord);
};