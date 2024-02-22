#ifndef GAMEIMPL_H
#define GAMEIMPL_H

#include <memory>

#include <QObject>

#include "helpers.h"

namespace tic_tac_toe {

class GameImpl;
class GameBoardImpl;
class Game : public QObject
{
    Q_OBJECT

public:
    Game(QObject *parent = nullptr);
    ~Game() override;

    helpers::PlayerInfo addPlayer(const QByteArray &name, helpers::PlayerMark mark,
                                  helpers::PlayerType playerType = helpers::PlayerType::Bot_Player);
    void removePlayer(int index);

    bool gameStatus() const;
    void changeGameStatus(bool status);

    GameBoardImpl *const board() const;

    void nextMove(int index = -1, helpers::NextMoveType type = helpers::NextMoveType::AutoFilled);

    bool hasWinner() const;
    QString winnerName() const;

    void reset();

signals:
    void gameStatusChanged(bool status);
    void winnerChanged(bool winner);

    void findError(helpers::error_handler::ErrorsType error);

private:
    bool isBotNextMove(const helpers::PlayerInfo &info) const;
    bool setWinner(const helpers::PlayerInfo &currentInfo);
    bool indexIsValid(int index, helpers::NextMoveType type);
    bool boardIsValid() const;
    void startGame();
    void resetWinner();
    void setDraw();

    std::unique_ptr<GameImpl> m_impl;

};

} // namespace tic_tac_toe
#endif // GAMEIMPL_H
