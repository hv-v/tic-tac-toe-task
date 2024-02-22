#include "models/GameBoardModel.h"
#include "app/GameImpl.h"

#include "app/GameBoardImpl.h"

namespace tic_tac_toe {

GameBoardModel::GameBoardModel(Game &gameImpl, GameBoardImpl* const boardImpl, QObject *parent)
    : QAbstractListModel(parent)
    , m_boardImpl(boardImpl)
    , m_gameImpl(gameImpl) {

    connect(m_boardImpl, &GameBoardImpl::cellValueChanged, this, [this](int raw) {
        const auto modelIndex = this->index(raw, 0);
        emit dataChanged(modelIndex, modelIndex, { MarkRole });
    });

    connect(m_boardImpl, &GameBoardImpl::boardUpdate, this, [this](){
        const auto fromIndex = this->index(0, 0);
        const auto toIndex = this->index(m_boardImpl->size() - 1, 0);
        emit dataChanged(fromIndex, toIndex, { MarkRole });
    });
}

int GameBoardModel::rowCount(const QModelIndex &parent) const {
    return m_boardImpl->size();
}

QVariant GameBoardModel::data(const QModelIndex &index, int role) const {
    if (!indexIsValid(index)) {
        return QVariant("Invalid index");
    }

    if (role == MarkRole) {
        return markToString(m_boardImpl->cellValue(index.row()));
    }

    return QVariant();
}

bool GameBoardModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!indexIsValid(index)) {
        return false;
    }

    if (role == NextMoveRole) {
        m_gameImpl.nextMove(index.row(), helpers::NextMoveType::HumanInput);
    }

    emit dataChanged(index, index, { NextMoveRole, MarkRole });
    return true;
}

QHash<int, QByteArray> GameBoardModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[MarkRole] = "cellMark";
    roles[NextMoveRole] = "nextMove";
    return roles;

}

bool GameBoardModel::indexIsValid(const QModelIndex &index) const {
    const bool rowIsValid = index.row() >= 0 && index.row() < m_boardImpl->size();
    return index.isValid() && rowIsValid;
}

} // namespace tic_tac_toe