#pragma once

#include "game.h"

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    enum Mode { VsMulti, VsAI };
    Q_ENUM(Mode)
    enum Starter { Player, AI };
    Q_ENUM(Starter)

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QTableWidget *board;
    Game *game;

    bool eventFilter(QObject */*object*/, QEvent */*event*/) override;

    void populateUi();
    void toggleOptions();
    void setupBoardUi();
    void resizeBoardUi();

    void gameReset();
    bool gameDoTurn(QPoint position);
    void gameDoTurnAi();
    bool gameIsTurnAi();
    Game::Player gameGetPlayer();
    Game::Player gameGetPlayerAi();
    void gameUpdateUi(bool success, Game::Player lastPlayer, QPoint position);

    Mode getMode();
    uint16_t getBoardSize();
    Starter getStarter();
    uint16_t getDifficulty();

    QString spaceCamelCase(const QString &s);

private slots:
    void on_tableWidgetBoard_cellClicked(int column, int row);
    void on_pushButtonReset_clicked();
    void on_comboBoxMode_currentIndexChanged(int /*index*/);
    void on_comboBoxStarter_currentIndexChanged(int /*index*/);
    void on_lineEditSize_textChanged(const QString &/*arg1*/);
};
