#include "minimax.h"

int Minimax::calcHeuristics(Game::CellList cells) {
    return 0; // TODO
}

QPoint Minimax::getAiTurn(Game game) {
    // TODO Minimax
    // Search for next empty cell (temporary)
    Game::CellList cells = game.getCells();
    auto it = std::find_if(cells.begin(), cells.end(), [](Game::Cell cell) { return cell.player == Game::Player::None; });
    if (it == cells.end()) {
        return {-1, -1};
    }
    return it->position;
}
