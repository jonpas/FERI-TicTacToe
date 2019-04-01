#include "game.h"

Game::Game(uint16_t boardSize_) :
        boardSize(boardSize_),
        maxTurns(boardSize_ * boardSize_) {
}

Game::Game(const Game &game) :
        QObject(),
        boardSize(game.boardSize),
        maxTurns(game.maxTurns),
        cells(game.cells),
        currentTurn(game.currentTurn),
        currentState(game.currentState),
        currentPlayer(game.currentPlayer) {
}

Game::~Game() {}

void Game::reset(Player startPlayer) {
    cells.clear();
    cells.reserve(boardSize * boardSize);
    for (int x = 0; x < boardSize; x++) {
        for (int y = 0; y < boardSize; y++) {
            cells.append({{x, y}, Player::None});
        }
    }

    currentTurn = 0;
    currentState = State::Running;

    currentPlayer = startPlayer;
}

void Game::stop(Player winner) {
    currentState = checkWinner(winner);
}

bool Game::doTurn(QPoint position) {
    Cell *cell = getCell(position);
    if (cell != nullptr && cell->player == Player::None) {
        cell->player = currentPlayer;

        // Advance to next player
        currentPlayer = (currentPlayer == Player::O) ? Player::X : Player::O;
        currentTurn++;

        State state = checkEnd();
        currentState = state;

        return true;
    }
    return false;
}

Game::State Game::checkEnd() {
    // Diagonal Top-Left <-> Bottom-Right
    Player winner = getCell({0, 0})->player;
    for (int x = 1; x < boardSize; x++) {
        if (getCell({x, x})->player != winner) {
            winner = Player::None;
            break;
        }
    }
    if (winner != Player::None) return checkWinner(winner);

    // Diagonal Bottom-Left <-> Top-Right
    winner = getCell({0, boardSize - 1})->player;
    for (int x = 1; x < boardSize; x++) {
        if (getCell({x, boardSize - 1 - x})->player != winner) {
            winner = Player::None;
            break;
        }
    }
    if (winner != Player::None) return checkWinner(winner);

    // Any full row
    for (int y = 0; y < boardSize; y++) {
        winner = getCell({0, y})->player;
        for (int x = 1; x < boardSize; x++) {
            if (getCell({x, y})->player != winner) {
                winner = Player::None;
                break;
            }
        }
        if (winner != Player::None) return checkWinner(winner);
    }

    // Any full column
    for (int x = 0; x < boardSize; x++) {
        winner = getCell({x, 0})->player;
        for (int y = 1; y < boardSize; y++) {
            if (getCell({x, y})->player != winner) {
                winner = Player::None;
                break;
            }
        }
        if (winner != Player::None) return checkWinner(winner);
    }

    // Draw
    if (currentTurn == maxTurns) {
        currentState = State::Draw;
        return State::Draw;
    }

    // Running
    return State::Running;
}

Game::Player Game::getCurrentPlayer() const {
    return currentPlayer;
}

Game::Player Game::getOtherPlayer() const {
    if (currentPlayer == Player::X) {
        return Player::O;
    }
    return Player::X;
}

Game::State Game::getCurrentState() const {
    return currentState;
}

bool Game::isOver() const {
    return currentState != State::Running;
}

Game::CellList Game::getCells() const {
    return cells;
}

Game::Cell *Game::getCell(QPoint position) {
    int index = position.y() + (position.x() * boardSize);
    if (index < cells.size() && index >= 0) {
        return &cells[index];
    } else {
        return nullptr;
    }
}

Game::Player Game::getWinnerFromState(State state) {
    if (state == State::XWin) {
        return Player::X;
    } else if (state == State::OWin) {
        return Player::O;
    }
    return Player::None;
}

Game::State Game::checkWinner(Player winner) {
    if (winner == Player::O) {
        return State::OWin;
    } else if (winner == Player::X) {
        return State::XWin;
    }
    return State::Running;
}
