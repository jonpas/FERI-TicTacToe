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
    // Stops game, setting given player as winner
    void stop(Player winner);
    // Performs turn with current player, returns false if wrong move, true if successful
    bool doTurn(QPoint position);
    // Checks if reached end state (player win)
    State checkEnd();

    Player getCurrentPlayer() const;
    Player getOtherPlayer() const;
    State getCurrentState() const;
    bool isOver() const;
    CellList getCells() const;
    Cell *getCell(QPoint position);

    static Player getWinnerFromState(State state);

private:
    CellList cells;
    uint32_t currentTurn;

    State currentState;
    Player currentPlayer;

    State checkWinner(Player winner);
};
