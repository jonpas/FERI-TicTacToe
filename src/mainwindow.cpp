#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaEnum>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    board = ui->tableWidgetBoard;
    board->installEventFilter(this);
    populateUi();
    setupBoardUi();

    game = new Game();
    resetGame();
}

MainWindow::~MainWindow() {
    for (int i = 0; i < board->rowCount(); i++) {
        for (int j = 0; j < board->columnCount(); j++) {
            delete board->item(i, j);
        }
    }

    delete game;
    delete ui;
}

bool MainWindow::eventFilter(QObject */*object*/, QEvent *event) {
    if (event->type() == QEvent::Resize) {
        resizeBoardUi();
    }
    return false;
}

void MainWindow::populateUi() {
    // Mode
    ui->comboBoxMode->blockSignals(true);
    ui->comboBoxMode->clear();
    QMetaEnum modePlacement = QMetaEnum::fromType<Mode>();
    for (int i = 0; i < modePlacement.keyCount(); i++) {
        ui->comboBoxMode->addItem(spaceCamelCase(modePlacement.key(i)));
    }
    ui->comboBoxMode->setCurrentIndex(Mode::Multi);
    ui->comboBoxMode->blockSignals(false);

    // TODO Line Edit validations
}

void MainWindow::setupBoardUi() {
    int size = Game::BoardSize;

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
}

void MainWindow::resizeBoardUi() {
    int boardSizeMin = board->size().width();
    if (board->size().height() < boardSizeMin) {
        boardSizeMin = board->size().width();
    }

    board->setFont(QFont("", boardSizeMin / 4));
}

void MainWindow::resetGame() {
    // Clear board
    for (int y = 0; y < board->rowCount(); y++) {
        for (int x = 0; x < board->columnCount(); x++) {
            QTableWidgetItem *item = board->item(x, y);
            item->setText("");
        }
    }

    game->reset();
}

MainWindow::Mode MainWindow::getMode() {
    return static_cast<Mode>(ui->comboBoxMode->currentIndex());
}

#include <QDebug>
void MainWindow::on_tableWidgetBoard_cellClicked(int x, int y) {
    if (game->isOver()) return;

    Game::Player lastPlayer = game->getCurrentPlayer();
    bool success = game->doTurn({y, x}); // Reversed!

    if (success) {
        QTableWidgetItem *item = board->item(x, y);

        if (lastPlayer == Game::Player::X) {
            item->setText("\u2717"); // Ballot X
            item->setForeground(Qt::black);
            ui->statusBar->showMessage(QString("Player Y turn!"), 10000);
        } else {
            item->setText("\u25ef"); // Large Circle
            item->setForeground(Qt::red);
            ui->statusBar->showMessage(QString("Player X turn!"), 10000);
        }

        if (game->isOver()) {
            ui->statusBar->showMessage(QString("Game over!"), 10000);
        }
    } else {
        ui->statusBar->showMessage(QString("Bad move!"), 10000);
    }
}

void MainWindow::on_pushButtonRestart_clicked() {
    resetGame();
}

QString MainWindow::spaceCamelCase(const QString &s) {
    static QRegularExpression regExp1 {"(.)([A-Z][a-z]+)"};
    static QRegularExpression regExp2 {"([a-z0-9])([A-Z])"};

    QString result = s;
    result.replace(regExp1, "\\1 \\2").replace(regExp2, "\\1 \\2");

    return result;
}
