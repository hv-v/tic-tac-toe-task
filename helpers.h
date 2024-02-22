#ifndef HELPERS_H
#define HELPERS_H

#include <QByteArray>
#include <QMetaEnum>

namespace tic_tac_toe::helpers {

namespace error_handler {

Q_NAMESPACE

enum class ErrorsType
{
    Invalid = -1,
    NameIsEmpty,
    MarkIsInvalid,
    TypeIsInvalid,
    PlayerTypeAlreadyExist,
    PlayerNameAlreadyExist,
    PlayerMarkAlreadyExist,
    FirstMarkIsCross,
};

Q_ENUM_NS(ErrorsType)

} // namespace error_handler

Q_NAMESPACE

enum class PlayerType
{
    Invalid = -1,
    Human_Player,
    Bot_Player,
};

Q_ENUM_NS(PlayerType)

enum class PlayerMark
{
    Invalid = -1,
    Empty,
    CrossMark,
    ZeroMark,
};

Q_ENUM_NS(PlayerMark)

struct PlayerInfo
{
    PlayerMark mark = PlayerMark::Invalid;
    QByteArray name;
    PlayerType type = PlayerType::Invalid;
};

enum class NextMoveType
{
    Invalid = -1,
    AutoFilled,
    HumanInput,
};

struct NextMoveInfo
{
    NextMoveType type = NextMoveType::Invalid;
    int index = -1;
};

inline QString markToString(tic_tac_toe::helpers::PlayerMark mr) {
    const std::unordered_map<tic_tac_toe::helpers::PlayerMark, QString> fromPlayerMark {
        { tic_tac_toe::helpers::PlayerMark::CrossMark, "X"},
        { tic_tac_toe::helpers::PlayerMark::ZeroMark, "O"},
        { tic_tac_toe::helpers::PlayerMark::Empty, " "},
    };

    return fromPlayerMark.at(mr);
}

template <typename Enum>
QString enumToString(Enum e) {
    const int value = static_cast<int>(e);
    return QMetaEnum::fromType<Enum>().valueToKey(value);
}

template<typename It>
inline bool lineFilter(It beginIt, helpers::PlayerMark playerMark, int offset) {
    const auto endIt = std::next(beginIt, offset);
    return std::all_of(beginIt, endIt, [&playerMark](const auto el) {
        return playerMark == el;
    });
}

template<typename It>
inline bool gapFilter(It beginIt, It endIt, helpers::PlayerMark playerMark, int gap) {
    bool result = true;
    while (beginIt < endIt) {
        result &= *beginIt == playerMark;
        beginIt = std::next(beginIt, gap);
    }

    return result;
}

inline bool isWinner(const std::vector<PlayerMark>& board, PlayerMark playerMark, int boardWidth) {
    for (auto it = board.begin(); it < board.end(); it = std::next(it, boardWidth) ) {
        if (lineFilter(it, playerMark, boardWidth)) {
            return true;
        }
    }

    for (auto it = board.begin(); it != std::next(board.begin(), boardWidth); ++it) {
        const auto endIt = std::next(std::prev(board.end(), boardWidth - 1), it - board.begin());
        if (gapFilter(it, endIt, playerMark, boardWidth)) {
            return true;
        }
    }

    if (gapFilter(board.begin(), board.end(), playerMark, boardWidth + 1)) {
        return true;
    }

    const auto rightTopIt = std::next(board.begin(), boardWidth - 1);
    const auto leftBottomIt = std::prev(board.end(), boardWidth - 1);
    if (gapFilter(rightTopIt, leftBottomIt, playerMark, boardWidth - 1)) {
        return true;
    }

    return false;
}


} // namespace tic_tac_toe::helpers

#endif // HELPERS_H
