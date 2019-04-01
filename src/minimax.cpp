#include "minimax.h"

#include <QDebug>

int Minimax::calcHeuristics(Game *game, Game::Player max, Game::Player min) {
    Game::State state = game->checkEnd();
    Game::Player winner = game->getWinnerFromState(state);

    int heuristics = 0;
    if (winner == max) {
        heuristics = 100; // TODO inf
    } else if (winner == min) {
        heuristics = -1; // TODO -inf
    } else {
        // Number of rows, columns and diagonals open for MAX
        // - number of rows, columns and diagonals open for MIN
        int hmax = calcHeuristicsOpen(game, /*enemy*/min);
        int hmin = calcHeuristicsOpen(game, /*enemy*/max);
        qDebug() << "max h =" << hmax << "| min h=" << hmin;
        heuristics = hmax - hmin;
    }

    qDebug() << "h =" << heuristics;
    return heuristics;
}

int Minimax::calcHeuristicsOpen(Game *game, Game::Player enemy)  {
    int h = 0;
    bool isEnemy;

    // Diagonal Top-Left <-> Bottom-Right
    isEnemy = false;
    for (int x = 0; x < game->boardSize; x++) {
        if (game->getCell({x, x})->player == enemy) {
            isEnemy = true;
            break;
        }
    }
    if (!isEnemy) h++;

    // Diagonal Bottom-Left <-> Top-Right
    isEnemy = false;
    for (int x = 0; x < game->boardSize; x++) {
        if (game->getCell({x, game->boardSize - 1 - x})->player == enemy) {
            isEnemy = true;
            break;
        }
    }
    if (!isEnemy) h++;

    // Any full row
    for (int y = 0; y < game->boardSize; y++) {
        isEnemy = false;
        for (int x = 0; x < game->boardSize; x++) {
            if (game->getCell({x, y})->player == enemy) {
                isEnemy = true;
                break;
            }
        }
        if (!isEnemy) h++;
    }

    // Any full column
    for (int x = 0; x < game->boardSize; x++) {
        isEnemy = false;
        for (int y = 0; y < game->boardSize; y++) {
            if (game->getCell({x, y})->player == enemy) {
                isEnemy = true;
                break;
            }
        }
        if (!isEnemy) h++;
    }

    return h;
}

QPoint Minimax::getAiTurn(Game game) {
    // TODO Minimax
    calcHeuristics(&game, game.getCurrentPlayer(), game.getOtherPlayer());

    // Search for next empty cell (temporary)
    Game::CellList cells = game.getCells();
    auto it = std::find_if(cells.begin(), cells.end(), [](Game::Cell cell) { return cell.player == Game::Player::None; });
    if (it == cells.end()) {
        return {-1, -1};
    }
    return it->position;
}
