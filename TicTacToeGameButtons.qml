import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root

    property alias gameStatus: internal.gameStatus

    signal startButtonSignal()
    signal resetButtonSignal()

    implicitHeight: internal.rootItemHeight
    implicitWidth: internal.rootItemWidth

    RowLayout {
        anchors.fill: parent

        Rectangle {
            id: startButton

            implicitWidth: parent.width / 2
            implicitHeight: parent.height

            Layout.alignment: Qt.AlignLeft

            radius: internal.defaultPopupRadius

            color: internal.backgroundColor

            border.width: internal.defaultBorderWidth
            border.color: internal.borderColor

            Text {
                anchors.centerIn: parent

                color: internal.startTextColor
                font.pixelSize: internal.defaultFontSize

                text: internal.startButtonText
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.startButtonSignal()
                }
            }
        }

        Rectangle {
            implicitWidth: parent.width / 2
            implicitHeight: parent.height

            Layout.alignment: Qt.AlignRight

            radius: internal.defaultPopupRadius

            color: internal.backgroundColor

            border.width: internal.defaultBorderWidth
            border.color: internal.borderColor

            Text {
                anchors.centerIn: parent

                color: internal.startTextColor
                font.pixelSize: internal.defaultFontSize

                text: "Reset"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.resetButtonSignal()
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

        readonly property int rootItemWidth: 300
        readonly property int rootItemHeight: 50

        readonly property int defaultPopupRadius: 10
        readonly property int defaultFontSize: 20
        readonly property int defaultBorderWidth: 2
    }
}
