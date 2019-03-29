#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    board = ui->tableWidgetBoard;
    board->installEventFilter(this);
    populateUi();

    cells.append({{1, 1}, Game::Player::O});
    cells.append({{1, 0}, Game::Player::X});
    setupBoardUi();
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

void MainWindow::populateUi() {
    // TODO Line Edit validations
}

void MainWindow::setupBoardUi() {
    int size = getBoardSize();

    board->setRowCount(size);
    board->setColumnCount(size);
    board->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    board->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    board->setStyleSheet("QTableWidget::item { padding: 0px }");

    for (int y = 0; y < board->rowCount(); y++) {
        for (int x = 0; x < board->columnCount(); x++) {
            QTableWidgetItem *item = board->item(x, y);

            // Create new item if it doesn't exist yet
            if (item == nullptr) {
                item = new QTableWidgetItem();
                item->setTextAlignment(Qt::AlignmentFlag::AlignCenter);
                board->setItem(x, y, item);
            }
        }
    }

    resizeBoardUi();
}

void MainWindow::resizeBoardUi() {
    int boardSizeMin = board->size().width();
    if (board->size().height() < boardSizeMin) {
        boardSizeMin = board->size().width();
    }

    board->setFont(QFont("", boardSizeMin / 4));
}

void MainWindow::setupBoard() {
    // Clear text
    for (int y = 0; y < board->rowCount(); y++) {
        for (int x = 0; x < board->columnCount(); x++) {
            QTableWidgetItem *item = board->item(x, y);

            item->setText("");
        }
    }

    // Set new text
    for (auto &cell : cells) {
        QTableWidgetItem *item = board->item(cell.pos.y(), cell.pos.x());
        if (cell.player == Game::Player::X) {
            item->setText("\u2717"); // Ballot X
            item->setForeground(Qt::black);
        } else {
            item->setText("\u25ef"); // Large Circle
            item->setForeground(Qt::red);
        }
    }
}
