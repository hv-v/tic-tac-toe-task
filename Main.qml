import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    title: internal.mainWindowName

    width: internal.mainWindowWidth
    height: internal.mainWindowHeight

    visible: true

    TicTacToeGame {
        id: ticTacToe

        anchors.fill: parent

        model: gameModel
    }

    QtObject {
        id: internal

        readonly property string mainWindowName: "Tic Tac Toe"
        readonly property int mainWindowWidth: 720
        readonly property int mainWindowHeight: 640
    }
}
