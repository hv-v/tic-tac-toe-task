#include "app/Player.h"

#include "app/GameBoardImpl.h"

namespace tic_tac_toe {

HumanPlayerVisitor::HumanPlayerVisitor(const QByteArray& name, helpers::PlayerMark mr)
    : PlayerVisitor(name, mr, helpers::PlayerType::Human_Player) {
}

HumanPlayerVisitor::~HumanPlayerVisitor() = default;

void HumanPlayerVisitor::playerMove(GameBoardImpl &board, const helpers::NextMoveInfo &info) const {
    if (info.type != helpers::NextMoveType::HumanInput) {
        return;
    }

    const helpers::PlayerMark mark = PlayerVisitor::getMark();

    const size_t index = info.index;
    board.action(index, mark);
}

}