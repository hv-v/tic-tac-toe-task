#include "app/Player.h"

#include "helpers.h"


namespace tic_tac_toe {

PlayerVisitor::PlayerVisitor(const QByteArray& name, const helpers::PlayerMark mark, const helpers::PlayerType type)
    : m_infoPtr(std::make_unique<helpers::PlayerInfo>()) {
    m_infoPtr->name = name;
    m_infoPtr->mark = mark;
    m_infoPtr->type = type;
}

PlayerVisitor *PlayerVisitor::create(const QByteArray& name, const helpers::PlayerMark mr, const helpers::PlayerType type)
{
    if (type == helpers::PlayerType::Bot_Player) {
        return new BotPlayerVisitor(name, mr);
    }

    if (type == helpers::PlayerType::Human_Player) {
        return new HumanPlayerVisitor(name, mr);
    }

    assert(false);
    return nullptr;
}

helpers::PlayerInfo PlayerVisitor::getPlayerInfo() const {
    return *m_infoPtr.get();
}

helpers::PlayerMark PlayerVisitor::getMark() const {
    return m_infoPtr->mark;
}

helpers::PlayerType PlayerVisitor::getPlayerType() const {
    return m_infoPtr->type;
}

PlayerVisitor::~PlayerVisitor() = default;

} // namespace tic_tac_toe