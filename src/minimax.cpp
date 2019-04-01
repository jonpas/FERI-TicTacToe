#include "minimax.h"

#include <QDebug>

int Minimax::calcHeuristics(Game *game, Game::Player max, Game::Player min) {
    Game::State state = game->checkEnd();
    Game::Player winner = game->getWinnerFromState(state);

    int heuristics = 0;
    if (winner == max) {
        heuristics = std::numeric_limits<int>::max(); // inf
    } else if (winner == min) {
        heuristics = std::numeric_limits<int>::min(); // -inf
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

QPoint Minimax::getAiTurn(Game game, uint16_t difficulty, int alpha, int beta) {
    // TODO Minimax
    Game::Player max = game.getCurrentPlayer();
    Game::Player min = game.getOtherPlayer();
    //calcHeuristics(&game, max, min);
    Result result = minimax(&game, max, max, min, difficulty, alpha, beta);
    return result.move;

    // Search for next empty cell (temporary)
    /*Game::CellList cells = game.getCells();
    auto it = std::find_if(cells.begin(), cells.end(), [](Game::Cell cell) { return cell.player == Game::Player::None; });
    if (it == cells.end()) {
        return {-1, -1};
    }
    return it->position;*/
}

Minimax::Result Minimax::minimax(Game *game, Game::Player currentPlayer, Game::Player max, Game::Player min, uint16_t depth, int alpha, int beta) {
    if (/*P je list ||*/ depth == 0) {
    //     return (f(P), NIL)
        return {calcHeuristics(game, max, min), {-1, -1}};
    }

    int score;
    if (currentPlayer == max) {
        score = std::numeric_limits<int>::min(); // -inf
    } else {
        score = std::numeric_limits<int>::max(); // inf
    }

    QPoint move;
    QList<QPoint> allowedMoves = getAllowedMoves(game);
    for (auto &allowedMove : allowedMoves) {
        // Copy game and do move
        Game moveGame(*game);
        moveGame.doTurn(allowedMove);

        // Run minimax on new move with other player
        Game::Player moveMax = moveGame.getCurrentPlayer();
        Game::Player moveMin = moveGame.getOtherPlayer();
        Result result = minimax(&moveGame, moveMax, moveMax, moveMin, depth - 1, alpha, beta);

        // Update best move if better than last found best move and track alpha-beta pruning
        if (currentPlayer == max && result.score > score) {
            score = result.score;
            move = allowedMove;
            if (score > alpha) alpha = score;
        }

        if (currentPlayer == min && result.score < score) {
            score = result.score;
            move = allowedMove;
            if (score < beta) beta = score;
        }

        // Cut alpha-beta (alpha-beta pruning)
        if (alpha >= beta) {
            return {score, move};
        }
    }

    return {score, move};
}

QList<QPoint> Minimax::getAllowedMoves(/*const*/ Game *game) {
    QList<QPoint> moves;

    for (int x = 0; x < game->boardSize; x++) {
        for (int y = 0; y < game->boardSize; y++) {
            if (game->getCell({x, y})->player == Game::Player::None) {
                moves.push_back({x, y});
            }
        }
    }

    return moves;
}
