#include "app/GameBoardImpl.h"

#include <string>

#include "app/Player.h"

namespace tic_tac_toe {

GameBoardImpl::GameBoardImpl(const int boardWidth, QObject *parent)
    : QObject(parent)
    , m_boardWidth(boardWidth)
    , m_board(m_boardWidth * m_boardWidth) {
    reset();
}

void GameBoardImpl::action(const int index, const helpers::PlayerMark playerMark) {
    if (m_board.at(index) != helpers::PlayerMark::Empty) {
        return;
    }

    m_board.at(index) = playerMark;
    cellValueChangedEmit(index);
}

void GameBoardImpl::nextMove(const PlayerVisitor &visitor, const helpers::NextMoveInfo &info) {
    visitor.playerMove(*this, info);
}

int GameBoardImpl::size() const {
    return m_board.size();
}

int GameBoardImpl::width() const {
    return m_boardWidth;
}

helpers::PlayerMark GameBoardImpl::cellValue(int index) const {
    return m_board.at(index);
}

const std::vector<helpers::PlayerMark> &GameBoardImpl::getBoard() const {
    return m_board;
}

void GameBoardImpl::reset() {
    std::fill(m_board.begin(), m_board.end(), helpers::PlayerMark::Empty);
    emit boardUpdate();
}

void GameBoardImpl::cellValueChangedEmit(int index) {
    emit cellValueChanged(index);
}

} // namespace tic_tac_toe