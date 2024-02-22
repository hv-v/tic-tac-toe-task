#ifndef GAMEBOARDIMPL_H
#define GAMEBOARDIMPL_H

#include <vector>

#include <QObject>

#include "helpers.h"

namespace tic_tac_toe {

class PlayerVisitor;

class GameBoardImpl : public QObject
{
    Q_OBJECT

    static constexpr int k_defaultSize = 3;

public:
    explicit GameBoardImpl(const int boardWidth = k_defaultSize, QObject* parent = nullptr);

    void action(const int index, const helpers::PlayerMark playerSign);
    void nextMove(const PlayerVisitor& visitor, const helpers::NextMoveInfo &info);

    int size() const;
    int width() const;

    helpers::PlayerMark cellValue(int index) const;
    const std::vector<helpers::PlayerMark>& getBoard() const;

    void reset();

signals:
    void cellValueChanged(int index);
    void boardUpdate();

private:
    void cellValueChangedEmit(int index);

    const int m_boardWidth;
    std::vector<helpers::PlayerMark> m_board;
};

} // namespace tic_tac_toe

#endif // GAMEBOARDIMPL_H