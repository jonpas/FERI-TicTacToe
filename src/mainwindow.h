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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QTableWidget *board;
    QList<Game::Cell> cells;

    bool eventFilter(QObject */*object*/, QEvent */*event*/) override;

    void populateUi();
    void setupBoardUi();
    void resizeBoardUi();
    void setupBoard();

    uint8_t getBoardSize() { return 3; }
};
