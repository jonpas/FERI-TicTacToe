#include "game.h"

Game::Game(uint16_t boardSize_) :
        boardSize(boardSize_),
        maxTurns(boardSize_ * boardSize_),
        startPlayer(Player::X) {
}

Game::~Game() {}

void Game::reset() {
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

bool Game::doTurn(QPoint position) {
    Cell *cell = getCell(position);
    if (cell->player == Player::None) {
        cell->player = currentPlayer;

        // Advance to next player
        currentPlayer = (currentPlayer == Player::O) ? Player::X : Player::O;
        currentTurn++;

        checkEnd();

        return true;
    }
    return false;
}

Game::Player Game::getCurrentPlayer() const {
    return currentPlayer;
}

bool Game::isOver() const {
    return currentState != State::Running;
}

Game::State Game::getCurrentState() const {
    return currentState;
}

void Game::checkEnd() {
    // Draw
    if (currentTurn == maxTurns) {
        currentState = State::Draw;
        return;
    }

    // TODO Check if any player won
    // currentState = State::OWin;
    // currentState = State::XWin;
}

Game::Cell *Game::getCell(QPoint position) {
    int index = position.y() + (position.x() * boardSize);
    if (index < cells.size()) {
        return &cells[index];
    } else {
        return nullptr;
    }
}
