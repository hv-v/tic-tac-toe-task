#ifndef PLAYER_H
#define PLAYER_H

#include <QByteArray>

#include "helpers.h"

namespace tic_tac_toe {

class GameBoardImpl;

class PlayerVisitor
{
public:
    explicit PlayerVisitor(const QByteArray& name, const helpers::PlayerMark sign, const helpers::PlayerType type);
    virtual ~PlayerVisitor();

    virtual void playerMove(GameBoardImpl &, const helpers::NextMoveInfo& info) const = 0;
    static PlayerVisitor * create(const QByteArray &name, const helpers::PlayerMark mr, const helpers::PlayerType type);

    helpers::PlayerInfo getPlayerInfo() const;

protected:
    helpers::PlayerMark getMark() const;
    helpers::PlayerType getPlayerType() const;

private:
    std::unique_ptr<helpers::PlayerInfo> m_infoPtr;
};

struct HumanPlayerVisitor : public PlayerVisitor
{
    explicit HumanPlayerVisitor(const QByteArray &name, helpers::PlayerMark mr);
    virtual ~HumanPlayerVisitor() override;

    virtual void playerMove(GameBoardImpl &board, const helpers::NextMoveInfo& info) const override;
};

class BotPlayerVisitor : public PlayerVisitor
{
public:
    explicit BotPlayerVisitor(const QByteArray &name, const helpers::PlayerMark ch);
    virtual ~BotPlayerVisitor() override;

    virtual void playerMove(GameBoardImpl &board, const helpers::NextMoveInfo& info) const override;
};

} // tic_tac_toe
#endif
