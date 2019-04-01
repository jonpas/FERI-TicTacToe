#pragma once

#include "game.h"

#include <QList>

namespace Minimax {
    struct Result {
        int score;
        QPoint move;
    };

    // Calculate heuristics
    int calcHeuristics(Game *game, Game::Player max, Game::Player min);
    // Counts number of rows, columns and diagonals open for given player
    int calcHeuristicsOpen(Game *game, Game::Player enemy);

    // Start minimax search, receives copy of current game
    QPoint getAiTurn(Game game, uint32_t difficulty);

    // Minimax algorithm
    Result minimax(Game *game, Game::Player currentPlayer, Game::Player max, Game::Player min, uint32_t depth, int alpha, int beta);

    // Gets all allowed moves (any empty cell)
    QList<QPoint> getAllowedMoves(/*const*/ Game *game);
};
