#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "minimax.h"

#include <QMetaEnum>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    board = ui->tableWidgetBoard;
    board->installEventFilter(this);
    populateUi();
    toggleOptions();

    game = new Game(getBoardSize());
    setupBoardUi();
    gameReset();
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
    ui->comboBoxMode->setCurrentIndex(Mode::VsAI);
    ui->comboBoxMode->blockSignals(false);

    // Starter
    ui->comboBoxStarter->blockSignals(true);
    ui->comboBoxStarter->clear();
    QMetaEnum starterPlacement = QMetaEnum::fromType<Starter>();
    for (int i = 0; i < starterPlacement.keyCount(); i++) {
        if (i == 2) { // Rename Player::None
            ui->comboBoxStarter->addItem("Random");
        } else {
            ui->comboBoxStarter->addItem(spaceCamelCase(starterPlacement.key(i)));
        }
    }
    ui->comboBoxStarter->setCurrentIndex(Starter::Player);
    ui->comboBoxStarter->blockSignals(false);

    // Line Edit validations
    const QIntValidator *validatorNaturalUInt16 = new QIntValidator(3, std::numeric_limits<uint16_t>::max(), this);

    ui->lineEditSize->setValidator(validatorNaturalUInt16);
    ui->lineEditDifficulty->setValidator(validatorNaturalUInt16);
}

void MainWindow::toggleOptions() {
    Mode mode = getMode();
    ui->optionsModeAIWidget->setHidden(mode != Mode::VsAI);
    ui->optionsModeMultiWidget->setHidden(mode != Mode::VsMulti);
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

void MainWindow::gameReset() {
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

    game->reset();
    if (gameIsTurnAi()) {
        gameDoTurnAi();
    }
}

bool MainWindow::gameDoTurn(QPoint position) {
    Game::Player lastPlayer = game->getCurrentPlayer();
    bool success = game->doTurn(position);
    gameUpdateUi(success, lastPlayer, position);

    if (success && !game->isOver() && gameIsTurnAi()) {
        gameDoTurnAi();
    }

    return success;
}

void MainWindow::gameDoTurnAi() {
    QPoint position = Minimax::getAiTurn(*game, getDifficulty());
    bool success = gameDoTurn(position);

    if (!success) {
        ui->statusBar->showMessage("Error! AI made invalid move! Reset!");
        game->stop(gameGetPlayer());
    }
}

bool MainWindow::gameIsTurnAi() {
    return getMode() == Mode::VsAI && game->getCurrentPlayer() == gameGetPlayerAi();
}

Game::Player MainWindow::gameGetPlayer() {
    Game::Player player = Game::Player::X;
    if (getStarter() == Starter::AI) {
        player = Game::Player::O;
    }
    return player;
}

Game::Player MainWindow::gameGetPlayerAi() {
    Game::Player aiPlayer = Game::Player::O;
    if (getStarter() == Starter::AI) {
        aiPlayer = Game::Player::X;
    }
    return aiPlayer;
}

void MainWindow::gameUpdateUi(bool success, Game::Player lastPlayer, QPoint position) {
    if (success) {
        QTableWidgetItem *item = board->item(position.y(), position.x()); // Table Widget reversed!

        // Mark turn in UI
        if (lastPlayer == Game::Player::X) {
            item->setText("\u2717"); // Ballot X
            item->setForeground(Qt::black);
            ui->statusBar->showMessage("Turn: Y");
        } else {
            item->setText("\u25ef"); // Large Circle
            item->setForeground(Qt::red);
            ui->statusBar->showMessage("Turn: X");
        }

        if (game->isOver()) {
            // Game over
            switch (game->getCurrentState()) {
                case Game::State::Draw: {
                    ui->statusBar->showMessage("Game over! Draw!");
                    break;
                }
                case Game::State::OWin: {
                    ui->statusBar->showMessage("Game over! O won!");
                    break;
                }
                case Game::State::XWin: {
                    ui->statusBar->showMessage("Game over! X won!");
                    break;
                }
                default: {}
            }
        }
    } else {
        // Invalid move
        ui->statusBar->showMessage("Invalid move!");
    }
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

MainWindow::Starter MainWindow::getStarter() {
    return static_cast<Starter>(ui->comboBoxStarter->currentIndex());
}

uint16_t MainWindow::getDifficulty() {
    uint16_t size = static_cast<uint16_t>(ui->lineEditDifficulty->text().toUInt());
    if (size < 3) size = 3; // Manual low-bound validation (validator doesn't handle it)
    if (size > getBoardSize()) size = getBoardSize(); // Manual board size validation
    ui->lineEditDifficulty->setText(QString::number(size));
    return size;
}

void MainWindow::on_tableWidgetBoard_cellClicked(int column, int row) {
    if (game->isOver()) {
        ui->statusBar->showMessage(QString("Game is over! Reset!"));
        return;
    }

    QPoint position(row, column); // Table Widget reversed!
    gameDoTurn(position);
}

void MainWindow::on_pushButtonReset_clicked() {
    ui->statusBar->showMessage("");
    gameReset();
}

void MainWindow::on_comboBoxMode_currentIndexChanged(int /*index*/) {
    toggleOptions();
    gameReset();
}

void MainWindow::on_comboBoxStarter_currentIndexChanged(int /*index*/) {
    gameReset();
}

void MainWindow::on_lineEditSize_textChanged(const QString &/*arg1*/) {
    gameReset();
}

QString MainWindow::spaceCamelCase(const QString &s) {
    static QRegularExpression regExp1 {"(.)([A-Z][a-z]+)"};
    static QRegularExpression regExp2 {"([a-z0-9])([A-Z])"};

    QString result = s;
    result.replace(regExp1, "\\1 \\2").replace(regExp2, "\\1 \\2");

    return result;
}
