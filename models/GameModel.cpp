#include "models/GameModel.h"

#include "models/GameBoardModel.h"
#include "models/PlayerConfigModel.h"
#include "app/GameImpl.h"

namespace tic_tac_toe {

ErrorHandlerModel::ErrorHandlerModel(QObject *parent)
    : QObject(parent) {
}

ErrorHandlerModel::~ErrorHandlerModel() = default;

bool ErrorHandlerModel::showPopup() const {
    return m_errorOp.has_value() && m_visible;
}

helpers::error_handler::ErrorsType ErrorHandlerModel::error() const {
    return m_errorOp.value_or(helpers::error_handler::ErrorsType::Invalid);
}

void ErrorHandlerModel::setShowPopup(bool status) {
    if (m_visible == status) {
        return;
    }

    m_visible = status;
    emit showPopupChanged(m_visible && m_errorOp.has_value());
}

void ErrorHandlerModel::reset() {
    m_errorOp = std::nullopt;
    setShowPopup(false);
}

void ErrorHandlerModel::setError(helpers::error_handler::ErrorsType error) {
    if (m_errorOp.has_value() && m_errorOp.value() == error) {
        return;
    }

    m_errorOp = error;
    setShowPopup(m_errorOp.has_value());
    emit errorChanged(error);
}

GameModel::GameModel(QObject *parent)
        : QObject(parent)
        , m_impl(std::make_unique<tic_tac_toe::Game>(this))
        , m_gameBoardModel(std::make_unique<GameBoardModel>(*m_impl.get(), m_impl->board(), this))
        , m_playerConfigModel(std::make_unique<PlayerConfigModel>(*m_impl.get(), this))
        , m_errorHandlerModel(std::make_unique<ErrorHandlerModel>(this))
{
    connect(m_impl.get(), &Game::gameStatusChanged, this, &GameModel::gameStatusChanged);
    connect(m_impl.get(), &Game::winnerChanged, this, &GameModel::hasWinnerChanged);
    connect(m_impl.get(), &Game::winnerChanged, this, [this] (bool hasWinner) {
        emit winnerNameChanged(m_impl->winnerName());
    });

    connect(m_impl.get(), &Game::findError, m_errorHandlerModel.get(), &ErrorHandlerModel::setError);
}

GameModel::~GameModel() = default;

bool GameModel::gameStatus() const {
    return m_impl->gameStatus();
}

void GameModel::setGameStatus(bool v) {
    m_impl->changeGameStatus(v);
}

bool GameModel::hasWinner() const {
    return m_impl->hasWinner();
}

QString GameModel::winnerName() const {
    return m_impl->winnerName();
}

QObject *GameModel::gameBoardModel() const {
    return m_gameBoardModel.get();
}

QObject *GameModel::playerConfigModel() const {
    return m_playerConfigModel.get();
}

QObject *GameModel::errorHandlerModel() const {
    return m_errorHandlerModel.get();
}

void GameModel::reset() {
    m_impl->reset();
}

void GameModel::nextMove(int index) const {
    m_impl->nextMove();
}

}