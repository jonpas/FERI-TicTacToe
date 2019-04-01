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
    enum Mode { Multi, AI };
    Q_ENUM(Mode)

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

    void resetGame();

    Mode getMode();
    uint16_t getBoardSize();
    Game::Player getStarter();
    uint16_t getDifficulty();

    QString spaceCamelCase(const QString &s);

private slots:
    void on_tableWidgetBoard_cellClicked(int column, int row);
    void on_pushButtonReset_clicked();
    void on_comboBoxMode_currentIndexChanged(int /*index*/);
};
