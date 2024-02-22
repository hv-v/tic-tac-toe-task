#include "app/GameImpl.h"

#include "app/GameBoardImpl.h"
#include "utils/Utils.h"
#include "app/Player.h"

namespace tic_tac_toe {

struct GameImpl {
    bool boardChanged = false;;
    bool gameStatus = false;
    GameBoardImpl board;
    PlayerList<PlayerVisitor *> playersList;
    std::optional<helpers::PlayerInfo> winner = std::nullopt;
};

Game::Game(QObject *parent)
    : QObject(parent)
    , m_impl(std::make_unique<GameImpl>()) {
}

Game::~Game() = default;

helpers::PlayerInfo Game::addPlayer(const QByteArray& name, helpers::PlayerMark mark, helpers::PlayerType playerType) {
    auto playerPtr = PlayerVisitor::create(name, mark, playerType);
    assert(playerPtr);

    const auto info = playerPtr->getPlayerInfo();
    m_impl->playersList.push_back(std::move(playerPtr));
    return info;
}

void Game::removePlayer(int index) {
    m_impl->playersList.erase(index);
}

bool Game::gameStatus() const {
    return m_impl->gameStatus;
}

void Game::changeGameStatus(bool status) {
    if (m_impl->gameStatus == status) {
        return;
    }

    if (status) {
        reset();
    }

    m_impl->gameStatus = status;
    emit gameStatusChanged(status);
    if (m_impl->gameStatus) {
        startGame();
    }
}

GameBoardImpl *const Game::board() const {
    return &m_impl->board;
}

void Game::nextMove(int index, helpers::NextMoveType type) {
    if (!gameStatus()) {
        return;
    }

    if (!indexIsValid(index, type)) {
        return;
    }

    const helpers::NextMoveInfo info = {type, index};
    const PlayerVisitor *const currentPlayer = *(m_impl->playersList.current());
    m_impl->board.nextMove(*currentPlayer, info);

    if (setWinner(currentPlayer->getPlayerInfo())) {
        return;
    }

    const PlayerVisitor *const  player = *(m_impl->playersList.next());

    if (isBotNextMove(player->getPlayerInfo())) {
        return nextMove();
    }
}

bool Game::hasWinner() const {
    return m_impl->winner.has_value();
}

QString Game::winnerName() const {
    if (!m_impl->winner) {
        return {};
    }

    return QString(m_impl->winner.value().name);
}

void Game::reset() {
    board()->reset();
    m_impl->playersList.reset();
    resetWinner();
    changeGameStatus(false);
}

bool Game::isBotNextMove(const helpers::PlayerInfo &info) const {
    return info.type == helpers::PlayerType::Bot_Player;
}

bool Game::setWinner(const helpers::PlayerInfo &currentInfo) {
    const auto& boardData = m_impl->board.getBoard();
    if (helpers::isWinner(boardData, currentInfo.mark, m_impl->board.width())) {
        const auto resultOp = std::make_optional(currentInfo);

        changeGameStatus(false);
        m_impl->winner = resultOp;
        emit winnerChanged(resultOp.has_value());
        return resultOp.has_value();
    }

    if (!boardIsValid()) {
        setDraw();
        return true;
    }

    return false;
}

bool Game::indexIsValid(int index, helpers::NextMoveType type) {
    return type == helpers::NextMoveType::AutoFilled ||
            (type == helpers::NextMoveType::HumanInput && m_impl->board.cellValue(index) == helpers::PlayerMark::Empty);
}

bool Game::boardIsValid() const {
    return std::any_of(m_impl->board.getBoard().begin(), m_impl->board.getBoard().end(), [this] (const auto el) {
        return el == helpers::PlayerMark::Empty;
    });
}

void Game::startGame() {
    if (m_impl->playersList.size() < 2) {
        changeGameStatus(false);
        return;
    }

    const auto currentInfo = (*(m_impl->playersList.current()))->getPlayerInfo();
    if (currentInfo.mark != helpers::PlayerMark::CrossMark) {
        emit findError(helpers::error_handler::ErrorsType::FirstMarkIsCross);
        changeGameStatus(false);
        return;
    }

    if (currentInfo.type == helpers::PlayerType::Human_Player || !m_impl->gameStatus) {
        return;
    }

    nextMove();
}

void Game::resetWinner() {
    m_impl->winner = std::nullopt;
    emit winnerChanged(false);
}

void Game::setDraw() {
    helpers::PlayerInfo info{helpers::PlayerMark::Invalid, "Draw", helpers::PlayerType::Invalid};
    m_impl->winner = info;
    emit winnerChanged(m_impl->winner.has_value());
    changeGameStatus(false);
}

} // namespace tic_tac_toe

