import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import com.tic_tac_toe.enums 1.0

Item {
    id: root

    signal addPlayerSignal(var playerNameStr, var type, var mark)
    signal removePlayerSignal(var playerIndex)

    implicitWidth: internal.rootItemWidth
    implicitHeight: internal.rootItemHeight

    property bool addPlayerSuccess: true
    property bool isDelegate: false
    property string playerName: ""
    property int playerTypeIndex: -1
    property int playerMarkIndex: -1
    property int currentIndex: -1

    RowLayout {
        id: editDelegate

        TextField {
            id: playerNameText

            text: isDelegate ? root.playerName : ""
            placeholderText: !isDelegate ? internal.defaultPlayerName : ""
            readOnly: isDelegate

            Rectangle {
                id: playerNameTextBackgound

                anchors.fill: parent

                visible: !root.addPlayerSuccess

                border.width: 2
                radius: 4

                color: "transparent"
                border.color: "red"
            }
        }

        ComboBox {
            id: playerTypeBox

            model: internal.defaultPlayerTypeModel
            currentIndex: isDelegate ? internal.toPlayerTypeModelIndex(playerTypeIndex) : 0
            enabled: !isDelegate

            Rectangle {
                id: playerTypeBoxBackgound

                anchors.fill: parent

                visible: !root.addPlayerSuccess

                border.width: 2
                radius: 4

                color: "transparent"
                border.color: "red"
            }
        }

        ComboBox {
            id: playerMarkBox
            model: internal.defaultPlayerMarkModel
            currentIndex: isDelegate ? internal.toPlayerMarkModelIndex(playerMarkIndex) : 0
            enabled: !isDelegate

            Rectangle {
                id: playerMarkBoxBackgound

                anchors.fill: parent

                visible: !root.addPlayerSuccess

                border.width: 2
                radius: 4

                color: "transparent"
                border.color: "red"
            }
        }

        Button {
            id: addPlayerButton

            text: internal.buttonText
            visible: !isDelegate
            onClicked: {
                internal.addPlayer(playerNameText.text, playerTypeBox.currentIndex, playerMarkBox.currentIndex);
            }
        }

        Button {
            id: removePlayerButton

            text: internal.removeButtonText
            visible: isDelegate
            onClicked: {
                root.removePlayerSignal(root.currentIndex)
            }
        }
    }

    QtObject {
        id: internal

        readonly property int rootItemWidth: 240
        readonly property int rootItemHeight: 40

        readonly property string buttonText: "Add Player"
        readonly property string removeButtonText: "Remove Player"
        readonly property string defaultMarkText: "Sign (X/O)"
        readonly property string defaultPlayerName: "Player Name"
        readonly property var defaultPlayerTypeModel:  ["Player Type", "Human_Player", "Bot_Player"]
        readonly property var defaultPlayerMarkModel: ["Player Mark", "X", "O"]

        function toPlayerTypeModelIndex(index) {
            return index + 1;
        }

        function toPlayerMarkModelIndex(index) {
            return index;
        }

        function addPlayer(name, typeIndex, markIndex) {
            if (!name) {
                playerNameTextBackgound.visible = true;
                return;
            }
            playerNameTextBackgound.visible = false;

            var playerType = _toPlayerTypeEnum(typeIndex);
            if (playerType === PlayerHelpers.Invalid) {
                playerTypeBoxBackgound.visible = true;
                return;
            }
            playerTypeBoxBackgound.visible = false;

            var playerMark = _toPlayerMarkEnum(markIndex);
            if (playerMark === PlayerHelpers.Invalid) {
                playerMarkBoxBackgound.visible = true;
                return;
            }
            playerMarkBoxBackgound.visible = false;

            root.addPlayerSignal(name, playerType, playerMark);
            _resetDelegate();
        }

        function _toPlayerTypeEnum(index) {
            var playerTypes = [PlayerHelpers.Invalid, PlayerHelpers.Human_Player, PlayerHelpers.Bot_Player];
            if (index > 0 && index < playerTypes.length) {
                return playerTypes[index];
            }
            return PlayerHelpers.Invalid;
        }

        function _toPlayerMarkEnum(index) {
            var playerMarks = [PlayerHelpers.Invalid, PlayerHelpers.CrossMark, PlayerHelpers.ZeroMark];
            if (index > 0 && index < playerMarks.length) {
                return playerMarks[index];
            }
            return PlayerHelpers.Invalid;
        }

        function _resetDelegate() {
            playerNameTextBackgound.visible = false;
            playerMarkBoxBackgound.visible = false;
            playerTypeBoxBackgound.visible = false;
            playerNameText.text = "";
            playerMarkBox.currentIndex = 0;
            playerTypeBox.currentIndex = 0;
        }
    }
}
