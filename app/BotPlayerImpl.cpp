#include "app/Player.h"

#include "app/GameBoardImpl.h"

namespace {

const int k_maxScore = 100;
const int k_defaultBestMove = -1;
const char * const k_defaultBotPrefix = "_Bot";

bool isBoardFull(const std::vector<tic_tac_toe::helpers::PlayerMark> & board) {
    using PlayerMark = tic_tac_toe::helpers::PlayerMark;
    return std::all_of(board.begin(), board.end(), [](const auto el) { return el != PlayerMark::Empty; });
}

bool makeTmpMove(std::vector<tic_tac_toe::helpers::PlayerMark>& board, int index, tic_tac_toe::helpers::PlayerMark mark) {
    if (board.at(index) == tic_tac_toe::helpers::PlayerMark::Empty) {
        board.at(index) = mark;
        return true;
    }

    return false;
};

void undoTmpMove(std::vector<tic_tac_toe::helpers::PlayerMark>& board, int index, tic_tac_toe::helpers::PlayerMark mark) {
    board.at(index) = tic_tac_toe::helpers::PlayerMark::Empty;
};

tic_tac_toe::helpers::PlayerMark findOpponentMark(const std::vector<tic_tac_toe::helpers::PlayerMark> & board,
                                                 tic_tac_toe::helpers::PlayerMark playerMark) {
    const auto it = std::find_if(board.begin(), board.end(), [playerMark](const auto mark){
        return mark != playerMark && mark != tic_tac_toe::helpers::PlayerMark::Empty;
    });

    if (it == board.end()) {
        return tic_tac_toe::helpers::PlayerMark::Invalid;
    }

    return *it;
}

std::optional<int> currentBoardState(const std::vector<tic_tac_toe::helpers::PlayerMark>& board,
            tic_tac_toe::helpers::PlayerMark playerMark,
            int width) {
    const auto oppMark = findOpponentMark(board, playerMark);

    if (tic_tac_toe::helpers::isWinner(board, playerMark, width)) {
        return k_maxScore;
    }

    if (tic_tac_toe::helpers::isWinner(board, oppMark, width)) {
        return -k_maxScore;
    }

    if (isBoardFull(board)) {
        return 0;
    }

    return std::nullopt;
}

int minimax(std::vector<tic_tac_toe::helpers::PlayerMark>& board,
            tic_tac_toe::helpers::PlayerMark playerMark,
            tic_tac_toe::helpers::PlayerMark opponentMark,
            int width,
            bool isMaximizingPlayer) {
    if (auto boardStateOp = currentBoardState(board, playerMark, width); boardStateOp) {
        return boardStateOp.value();
    }

    if (isMaximizingPlayer) {
        int bestValue = std::numeric_limits<int>::min();
        for (int index = 0; index < board.size(); ++index) {

            if (!makeTmpMove(board, index, playerMark)) {
                continue;
            }

            const int value = minimax(board, playerMark, opponentMark, width, false);
            bestValue = std::max(bestValue, value);

            undoTmpMove(board, index, playerMark);
        }

        return bestValue;
    }

    int bestValue = std::numeric_limits<int>::max();
    for (int index = 0; index < board.size(); ++index) {

        if (!makeTmpMove(board, index, opponentMark)) {
            continue;
        }

        const int value = minimax(board, playerMark, opponentMark, width, true);
        bestValue = std::min(bestValue, value);

        undoTmpMove(board, index, playerMark);
    }

    return bestValue;
}

int bestMove(std::vector<tic_tac_toe::helpers::PlayerMark>& board,
                tic_tac_toe::helpers::PlayerMark playerMark,
                int width) {

    using PlayerMark = tic_tac_toe::helpers::PlayerMark;

    const int centerIndex = (board.size() - 1) / 2;
    if (board[centerIndex] == PlayerMark::Empty) {
        return centerIndex;
    }

    const auto oppMark = findOpponentMark(board, playerMark);

    int bestValue = std::numeric_limits<int>::min();
    int bestMove = k_defaultBestMove;

    for (int index = 0; index < board.size(); ++index) {
        if (!makeTmpMove(board, index, playerMark)) {
            continue;
        }

        const int moveValue = minimax(board, playerMark, oppMark, width, false);

        undoTmpMove(board, index, playerMark);
        if (moveValue > bestValue) {
            bestMove = index;
            bestValue = moveValue;
        }
    }

    return bestMove;
}

} // namespace

namespace tic_tac_toe {

BotPlayerVisitor::BotPlayerVisitor(const QByteArray &name, const helpers::PlayerMark mr)
    : PlayerVisitor(name + k_defaultBotPrefix, mr, helpers::PlayerType::Bot_Player) {

}

BotPlayerVisitor::~BotPlayerVisitor() = default;

void BotPlayerVisitor::playerMove(GameBoardImpl &board, const helpers::NextMoveInfo &info) const {
    if (info.type != helpers::NextMoveType::AutoFilled) {
        return;
    }

    const helpers::PlayerMark mark = PlayerVisitor::getMark();
    std::vector<helpers::PlayerMark> boardData = board.getBoard();
    int index = bestMove(boardData, mark, board.width());

    assert(index != k_defaultBestMove);
    board.action(index, mark);
}


} // namespace tic_tac_toe