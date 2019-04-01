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

    // TODO Calculate heuristics
    int calcHeuristics(Game::CellList cells);

    // Start minimax search, receives copy of current game
    QPoint getAiTurn(Game game);
};
