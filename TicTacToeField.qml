import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.15

Item {
    id: root

    property QtObject model: null // GameBoardModel.h

    implicitWidth: internal.gameBoardSize
    implicitHeight: internal.gameBoardSize

    GridView {
        id: gameBoardView

        anchors.fill: parent

        model: root.model

        delegate: Rectangle {
            implicitWidth: gameBoardView.width / 3
            implicitHeight: gameBoardView.height / 3

            color: internal.gameBoardColor

            border.color: internal.borderDefaultColor
            border.width: internal.borderWidth

            Text {
                anchors.centerIn: parent
                text: model.cellMark
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    model.nextMove = index;
                }
            }
        }
    }

    QtObject {
        id: internal

        readonly property color gameBoardColor: "lightgrey"
        readonly property color borderDefaultColor: "black"
        readonly property int gameBoardSize: 300
        readonly property int borderWidth: 2
    }

}

