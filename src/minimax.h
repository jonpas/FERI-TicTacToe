#pragma once

#include "game.h"

#include <QList>

namespace Minimax {
    struct State {
        Game::CellList cells;
        int heuristics;
        int steps = 0;

        bool operator<(const State &s) const {
            return heuristics < s.heuristics;
        }
    };

    // Calculate heuristics
    int calcHeuristics(Game *game, Game::Player max, Game::Player min);
    // Counts number of rows, columns and diagonals open for given player
    int calcHeuristicsOpen(Game *game, Game::Player enemy);

    // Start minimax search, receives copy of current game
    QPoint getAiTurn(Game game);
};
