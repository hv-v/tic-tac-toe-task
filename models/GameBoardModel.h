#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QAbstractListModel>

namespace tic_tac_toe  {

class Game;
class GameBoardImpl;
class GameBoardModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit GameBoardModel(Game& m_gameImpl, GameBoardImpl* const boardImpl, QObject *parent = nullptr);

    enum CellRoles {
        MarkRole = Qt::UserRole + 1,
        NextMoveRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    bool indexIsValid(const QModelIndex& index) const;

    GameBoardImpl* const m_boardImpl;
    Game& m_gameImpl;
};

} // namespace tic_tac_toe

#endif // GAMEMODEL_H
