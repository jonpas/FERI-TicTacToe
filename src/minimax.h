#pragma once

#include "game.h"

#include <QList>

namespace Minimax {
    struct State {
        QList<Game::Cell> cells;
        int heuristics;
        int steps = 0;

        bool operator<(const State &s) const {
            return heuristics < s.heuristics;
        }
    };

    // TODO Calculate heuristics
    int calcHeuristics(QList<Game::Cell> cells);
};
