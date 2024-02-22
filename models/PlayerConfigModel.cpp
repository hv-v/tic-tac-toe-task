#include "PlayerConfigModel.h"

#include <unordered_map>

#include "app/GameImpl.h"
#include "helpers.h"


namespace tic_tac_toe {

PlayerConfigModel::PlayerConfigModel(Game &gameImpl, QObject *parent)
    : QAbstractListModel(parent)
    , m_gameImpl(gameImpl) {
    initDefaultPlayerConfig();
}

PlayerConfigModel::~PlayerConfigModel() = default;

QHash<int, QByteArray> PlayerConfigModel::roleNames() const  {
    QHash<int, QByteArray> roles;
    roles[PlayerNameRole] = "playerName";
    roles[PlayerMarkRole] = "playerMark";
    roles[PlayerTypeRole] = "playerType";
    return roles;
}

bool PlayerConfigModel::addPlayer(const QString& playerName, helpers::PlayerMark mark, helpers::PlayerType type) {
    if (const auto errorOp = checkErrors(playerName, mark, type); errorOp) {
        emit m_gameImpl.findError(errorOp.value());
        return false;
    }

    const QByteArray name = playerName.toUtf8();

    const int insertIndex = PlayerConfigModel::rowCount();
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    const auto info = m_gameImpl.addPlayer(name, mark, type);
    m_playersInfo.push_back(info);
    endInsertRows();

    return true;
}

void PlayerConfigModel::removePlayer(const int rawIndex) {
    if (rawIndex < 0 || rawIndex >= m_playersInfo.size()) {
        return;
    }

    if (m_gameImpl.gameStatus()) {
        emit m_gameImpl.findError(helpers::error_handler::ErrorsType::RunningStatus);
        return;
    }


    beginRemoveRows(QModelIndex(), rawIndex, rawIndex);
    m_gameImpl.removePlayer(rawIndex);
    m_playersInfo.erase(std::next(m_playersInfo.begin(), rawIndex));
    endRemoveRows();
}

void PlayerConfigModel::initDefaultPlayerConfig() {
    addPlayer("Player1", helpers::PlayerMark::CrossMark, helpers::PlayerType::Human_Player);
    addPlayer("Player2", helpers::PlayerMark::ZeroMark, helpers::PlayerType::Bot_Player);
}

int PlayerConfigModel::rowCount(const QModelIndex &parent) const {
    return m_playersInfo.size();
}

QVariant PlayerConfigModel::data(const QModelIndex &index, int role) const  {
    if (!index.isValid() || index.row() >= m_playersInfo.size()) {
        return QVariant();
    }

    const auto &playerInfo = m_playersInfo.at(index.row());
    switch (role) {
        case PlayerNameRole:
            return QString(playerInfo.name);
        case PlayerMarkRole:
            return QVariant::fromValue(playerInfo.mark);
        case PlayerTypeRole:
            return QVariant::fromValue(playerInfo.type);
        default:
            return QVariant();
    }
}

std::optional<helpers::error_handler::ErrorsType> PlayerConfigModel::checkErrors(const QString& playerName,
                                                                      helpers::PlayerMark mark,
                                                                      helpers::PlayerType type) const {
    using ErrorsType = helpers::error_handler::ErrorsType;

    if (m_gameImpl.gameStatus()) {
        return ErrorsType::RunningStatus;
    }

    if (playerName.isEmpty()) {
        return ErrorsType::NameIsEmpty;
    }

    if (mark == helpers::PlayerMark::Invalid ) {
        return ErrorsType::MarkIsInvalid;
    }

    if ( type == helpers::PlayerType::Invalid)  {
        return ErrorsType::TypeIsInvalid;
    }

    const bool isNameExist = std::any_of(m_playersInfo.begin(), m_playersInfo.end(), [&playerName] (const auto& el) {
        return el.name == playerName;
    });

    if (isNameExist) {
        return ErrorsType::PlayerNameAlreadyExist;
    }

    const bool isMarkExist = std::any_of(m_playersInfo.begin(), m_playersInfo.end(), [mark] (const auto& el) {
        return el.mark == mark;
    });

    if (isMarkExist) {
        return ErrorsType::PlayerMarkAlreadyExist;
    }

    const bool isTypeExist = std::any_of(m_playersInfo.begin(), m_playersInfo.end(), [type] (const auto& el) {
        return el.type == type;
    });

    if (isTypeExist) {
        return ErrorsType::PlayerTypeAlreadyExist;
    }

    if (m_playersInfo.empty() && mark != helpers::PlayerMark::CrossMark) {
        return ErrorsType::FirstMarkIsCross;
    }

    return std::nullopt;
}

}