#pragma once

#include <random>
#include <QList>

class Game : public QObject {
    Q_OBJECT

public:
    static std::mt19937 randGen;
    static std::uniform_real_distribution<> distProbability;

    enum Player { O, X, None };
    Q_ENUM(Player)
    enum State { Running, XWin, OWin, Draw };
    Q_ENUM(State)

    struct Cell {
        QPoint position;
        Player player;
    };
    typedef QList<Cell> CellList;

    const uint16_t boardSize;
    const uint32_t maxTurns;

    Game(uint16_t boardSize_ = 3);
    ~Game();

    // Resets game
    void reset(Player startPlayer = Player::X);

    // Performs turn with current player, returns false if wrong move, true if successful
    bool doTurn(QPoint position);

    Player getCurrentPlayer() const;
    bool isOver() const;
    State getCurrentState() const;

private:
    CellList cells;
    uint32_t currentTurn;

    State currentState;
    Player currentPlayer;

    // Checks if reached end state (player win), returns
    State checkEnd();
    State checkWinner(Player winner);

    Cell *getCell(QPoint position);
};
