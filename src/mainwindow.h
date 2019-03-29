#pragma once

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QTableWidget *board;
    QList<QPoint> cells;

    bool eventFilter(QObject */*object*/, QEvent */*event*/) override;

    void resizeBoardUi();
    inline uint8_t getBoardSize() { return 3; }
    void populateUi();
    void setupBoard();
};
