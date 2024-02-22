#include <memory>
#include <QObject>

#include "helpers.h"

namespace tic_tac_toe {

class Game;
class GameBoardModel;
class PlayerConfigModel;

class ErrorHandlerModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool showPopup READ showPopup WRITE setShowPopup NOTIFY showPopupChanged)
    Q_PROPERTY(helpers::error_handler::ErrorsType error READ error NOTIFY errorChanged)

public:
    explicit ErrorHandlerModel(QObject* parent = nullptr);
    ~ErrorHandlerModel() override;

    bool showPopup() const;
    helpers::error_handler::ErrorsType error() const;
    void setShowPopup(bool status);

    Q_INVOKABLE void reset();

public slots:
    void setError(helpers::error_handler::ErrorsType error);

signals:
    void showPopupChanged(bool showPopup);
    void errorChanged(helpers::error_handler::ErrorsType error);

private:
    bool m_visible = false;
    std::optional<helpers::error_handler::ErrorsType> m_errorOp = std::nullopt;
};

class GameModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QObject* gameBoardModel READ gameBoardModel CONSTANT)
    Q_PROPERTY(QObject* playerConfigModel READ playerConfigModel CONSTANT)
    Q_PROPERTY(QObject* errorHandlerModel READ errorHandlerModel CONSTANT)

    Q_PROPERTY(bool gameStatus READ gameStatus WRITE setGameStatus NOTIFY gameStatusChanged)
    Q_PROPERTY(bool hasWinner READ hasWinner NOTIFY hasWinnerChanged)
    Q_PROPERTY(QString winnerName READ winnerName NOTIFY winnerNameChanged)

public:
    explicit GameModel(QObject *parent = nullptr);

    ~GameModel() override;

    bool gameStatus() const;
    void setGameStatus(bool v);

    bool hasWinner() const;
    QString winnerName() const;

    QObject *gameBoardModel() const;
    QObject *playerConfigModel() const;
    QObject *errorHandlerModel() const;

    Q_INVOKABLE void reset();


signals:
    void gameStatusChanged(bool status);
    void hasWinnerChanged(bool hasWinner);
    void winnerNameChanged(QString name);

private:
    void nextMove(int index) const;

    std::unique_ptr<Game> m_impl;
    std::unique_ptr<GameBoardModel> m_gameBoardModel;
    std::unique_ptr<PlayerConfigModel> m_playerConfigModel;
    std::unique_ptr<ErrorHandlerModel> m_errorHandlerModel;
};

}
