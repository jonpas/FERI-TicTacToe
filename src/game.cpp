#include "game.h"

std::mt19937 Game::randGen = std::mt19937{std::random_device{}()};
std::uniform_real_distribution<> Game::distProbability = std::uniform_real_distribution<>(0.0, 1.0);

Game::Game(uint16_t boardSize_) :
        boardSize(boardSize_),
        maxTurns(boardSize_ * boardSize_) {
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
    if (currentPlayer == Player::None) {
        currentPlayer = (distProbability(randGen) > 0.5) ? Player::X : Player::O;
    }
}

bool Game::doTurn(QPoint position) {
    Cell *cell = getCell(position);
    if (cell->player == Player::None) {
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

Game::Player Game::getCurrentPlayer() const {
    return currentPlayer;
}

bool Game::isOver() const {
    return currentState != State::Running;
}

Game::State Game::getCurrentState() const {
    return currentState;
}

Game::State Game::checkEnd() {
    // Draw
    if (currentTurn == maxTurns) {
        currentState = State::Draw;
        return State::Draw;
    }

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

    return State::Running;
}

Game::State Game::checkWinner(Player winner) {
    if (winner == Player::O) {
        return State::OWin;
    } else if (winner == Player::X) {
        return State::XWin;
    }
    return State::Running;
}

Game::Cell *Game::getCell(QPoint position) {
    int index = position.y() + (position.x() * boardSize);
    if (index < cells.size()) {
        return &cells[index];
    } else {
        return nullptr;
    }
}
