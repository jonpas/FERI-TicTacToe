#pragma once

#include <random>
#include <QList>

class Game : public QObject {
    Q_OBJECT

public:
    enum Player { X, O, None };
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
    Game(const Game &game);
    ~Game();

    // Resets game
    void reset(Player startPlayer = Player::X);
    void stop(Player winner);

    // Performs turn with current player, returns false if wrong move, true if successful
    bool doTurn(QPoint position);

    Player getCurrentPlayer() const;
    bool isOver() const;
    State getCurrentState() const;
    CellList getCells() const;

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
