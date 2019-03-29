#include "game.h"

const uint8_t Game::BoardSize = 3;
const uint8_t Game::MaxTurns = 9;

Game::Game() {}

Game::~Game() {}

void Game::reset() {
    cells.clear();
    cells.reserve(BoardSize * BoardSize);
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            cells.append({{x, y}, Player::None});
        }
    }

    currentTurn = 0;

    currentState = State::Running;
    currentPlayer = Player::X; // TODO Random? or UI setting (Random, X, O)
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
    if (currentTurn == MaxTurns) {
        currentState = State::Draw;
        return;
    }

    // TODO Check if any player won
    // currentState = State::OWin;
    // currentState = State::XWin;
}

Game::Cell *Game::getCell(QPoint position) {
    int index = position.y() + (position.x() * BoardSize);
    if (index < cells.size()) {
        return &cells[index];
    } else {
        return nullptr;
    }
}
