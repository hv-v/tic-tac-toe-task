#ifndef PLAYERCONFIGMODEL_H
#define PLAYERCONFIGMODEL_H

#include <QAbstractListModel>

#include "helpers.h"

namespace tic_tac_toe {

class Game;

class PlayerConfigModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PlayerConfigModel(Game &gameImpl, QObject*parent = nullptr);
    ~PlayerConfigModel() override;

    enum PlayerModelRole {
        PlayerNameRole = Qt::UserRole + 1,
        PlayerIndexRole,
        PlayerMarkRole,
        PlayerTypeRole,
    };
    Q_ENUM(PlayerModelRole)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE bool addPlayer(const QString &playerName, helpers::PlayerMark mark, helpers::PlayerType playerType);
    Q_INVOKABLE void removePlayer(const int index);

private:
    void initDefaultPlayerConfig();

    std::optional<helpers::error_handler::ErrorsType>
    checkErrors(const QString &, helpers::PlayerMark, helpers::PlayerType) const;

    std::vector<helpers::PlayerInfo> m_playersInfo;
    Game &m_gameImpl;
};

} // tic_tac_toe

#endif // PLAYERCONFIGMODEL_H
