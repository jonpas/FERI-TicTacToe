#pragma once

#include <QPoint>

namespace Game {
    enum Player { O, X, None };

    struct Cell {
        QPoint pos;
        Player player;
    };
}
