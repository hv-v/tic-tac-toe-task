import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root

    property QtObject model: null // PlayerConfigModel.h

    property alias gameStatus: internal.gameStatus

    signal startButtonSignal()
    signal resetButtonSignal()

    implicitHeight: internal.gameConfigHeight
    implicitWidth: internal.gameConfigWidth

    ColumnLayout {
        id: playersListLayout

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        spacing: internal.itemSpacing

        TicTacToePlayerDelegate {
            id: addPlayerDelegate

            onAddPlayerSignal: {
                addPlayerDelegate.addPlayerSuccess = model.addPlayer(playerNameStr, mark, type);
            }
        }

        ListView {
            id: playersList

            implicitHeight: 80
            implicitWidth: parent.width

            model: root.model

            delegate: TicTacToePlayerDelegate {
                isDelegate: true
                currentIndex: model.index

                playerName: model.playerName
                playerTypeIndex: model.playerType
                playerMarkIndex: model.playerMark

                onRemovePlayerSignal: {
                    root.model.removePlayer(playerIndex)
                }
            }
        }
    }

    QtObject {
        id: internal

        readonly property color backgroundColor: "#444444"
        readonly property color borderColor: "green"
        readonly property color startTextColor: "#FFFFFF"

        property bool gameStatus: false
        property string startButtonText: gameStatus ? "Stop Game" : "Start Game"

        readonly property int itemSpacing: 10
        readonly property int defaultPopupRadius: 10
        readonly property int defaultFontSize: 20
        readonly property int defaultBorderWidth: 2

        readonly property int gameConfigWidth: 300
        readonly property int gameConfigHeight: 150
    }
}
