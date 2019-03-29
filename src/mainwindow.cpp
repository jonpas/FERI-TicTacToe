#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "minimax.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    board = ui->tableWidgetBoard;
    board->installEventFilter(this);
    populateUi();

    setupBoard();
}

MainWindow::~MainWindow() {
    for (int i = 0; i < board->rowCount(); i++) {
        for (int j = 0; j < board->columnCount(); j++) {
            delete board->item(i, j);
        }
    }

    delete ui;
}

bool MainWindow::eventFilter(QObject */*object*/, QEvent *event) {
    if (event->type() == QEvent::Resize) {
        resizeBoardUi();
    }
    return false;
}

void MainWindow::resizeBoardUi() {
    board->setIconSize(board->size() / getBoardSize());
}

void MainWindow::populateUi() {
    // TODO Line Edit validations
}

void MainWindow::setupBoard() {
    int size = getBoardSize();

    board->setRowCount(size);
    board->setColumnCount(size);
    board->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    board->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int y = 0; y < board->rowCount(); y++) {
        for (int x = 0; x < board->columnCount(); x++) {
            QTableWidgetItem *item = board->item(x, y);

            // Create new item if it doesn't exist yet
            if (item == nullptr) {
                item = new QTableWidgetItem();
                board->setItem(x, y, item);
            }

            // Clear image
            item->setIcon(QIcon());
        }
    }

    for (auto &cell : cells) {
        QTableWidgetItem *item = board->item(cell.y(), cell.x());
        //item->setIcon(QIcon(":/rsc/crown.png"));
    }
}
