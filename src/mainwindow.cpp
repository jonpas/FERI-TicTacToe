#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaEnum>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    board = ui->tableWidgetBoard;
    board->installEventFilter(this);
    populateUi();

    game = new Game(getBoardSize());
    setupBoardUi();
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

    // Starter
    ui->comboBoxStarter->blockSignals(true);
    ui->comboBoxStarter->clear();
    QMetaEnum starterPlacement = QMetaEnum::fromType<Game::Player>();
    for (int i = 0; i < starterPlacement.keyCount(); i++) {
        if (i == 2) { // Rename Player::None to "Random"
            ui->comboBoxStarter->addItem("Random");
        } else {
            ui->comboBoxStarter->addItem(spaceCamelCase(starterPlacement.key(i)));
        }
    }
    ui->comboBoxStarter->setCurrentIndex(Game::Player::X);
    ui->comboBoxStarter->blockSignals(false);

    // Line Edit validations
    const QIntValidator *validatorNaturalUInt16 = new QIntValidator(3, std::numeric_limits<uint16_t>::max(), this);

    ui->lineEditSize->setValidator(validatorNaturalUInt16);
    ui->lineEditDifficulty->setValidator(validatorNaturalUInt16);
}

void MainWindow::setupBoardUi() {
    int size = game->boardSize;

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

    board->setFont(QFont("", static_cast<int>(boardSizeMin / 1.6) / game->boardSize));
}

void MainWindow::resetGame() {
    if (getBoardSize() != game->boardSize) {
        // Full reset (board size changed)
        delete game;
        game = new Game(getBoardSize());
        setupBoardUi();
    }

    // Clear board
    for (int y = 0; y < board->rowCount(); y++) {
        for (int x = 0; x < board->columnCount(); x++) {
            QTableWidgetItem *item = board->item(x, y);
            item->setText("");
        }
    }

    game->reset(getStarter());
}

MainWindow::Mode MainWindow::getMode() {
    return static_cast<Mode>(ui->comboBoxMode->currentIndex());
}

uint16_t MainWindow::getBoardSize() {
    uint16_t size = static_cast<uint16_t>(ui->lineEditSize->text().toUInt());
    if (size < 3) size = 3; // Manual low-bound validation (validator doesn't handle it)
    ui->lineEditSize->setText(QString::number(size));
    return size;
}

Game::Player MainWindow::getStarter() {
    return static_cast<Game::Player>(ui->comboBoxStarter->currentIndex());
}

uint16_t MainWindow::getDifficulty() {
    uint16_t size = static_cast<uint16_t>(ui->lineEditDifficulty->text().toUInt());
    if (size < 3) size = 3; // Manual low-bound validation (validator doesn't handle it)
    if (size > getBoardSize()) size = getBoardSize(); // Manual board size validation
    ui->lineEditDifficulty->setText(QString::number(size));
    return size;
}

void MainWindow::on_tableWidgetBoard_cellClicked(int column, int row) {
    if (game->isOver()) return;

    QPoint position(row, column); // Table Widget reversed!

    Game::Player lastPlayer = game->getCurrentPlayer();
    bool success = game->doTurn(position);

    if (success) {
        QTableWidgetItem *item = board->item(column, row);

        // Mark turn in UI
        if (lastPlayer == Game::Player::X) {
            item->setText("\u2717"); // Ballot X
            item->setForeground(Qt::black);
            ui->statusBar->showMessage(QString("Turn: Y"));
        } else {
            item->setText("\u25ef"); // Large Circle
            item->setForeground(Qt::red);
            ui->statusBar->showMessage(QString("Turn: X"));
        }

        if (game->isOver()) {
            // Game over
            switch (game->getCurrentState()) {
                case Game::State::Draw: {
                    ui->statusBar->showMessage(QString("Game over! Draw!"));
                    break;
                }
                case Game::State::OWin: {
                    ui->statusBar->showMessage(QString("Game over! O wins!"));
                    break;
                }
                case Game::State::XWin: {
                    ui->statusBar->showMessage(QString("Game over! X wins!"));
                    break;
                }
                default: {}
            }
        } else if (getMode() == Mode::AI && game->getCurrentPlayer() == Game::Player::O) {
            // AI mode and next turn is of AI
            // TODO call AI magic, returns position of next turn
            // TODO Split all above
            // TODO - doTurn (split from above)
            // TODO - Mark turn in UI (split from above)
            // TODO - Game over messages (split from above)
        }
    } else {
        // Invalid move
        ui->statusBar->showMessage(QString("Invalid move!"), 10000);
    }
}

void MainWindow::on_pushButtonReset_clicked() {
    resetGame();
}

QString MainWindow::spaceCamelCase(const QString &s) {
    static QRegularExpression regExp1 {"(.)([A-Z][a-z]+)"};
    static QRegularExpression regExp2 {"([a-z0-9])([A-Z])"};

    QString result = s;
    result.replace(regExp1, "\\1 \\2").replace(regExp2, "\\1 \\2");

    return result;
}
