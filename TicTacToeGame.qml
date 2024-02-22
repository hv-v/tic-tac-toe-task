import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import com.tic_tac_toe.enums 1.0
import com.tic_tac_toe.error_types 1.0

Item {
    id: root

    property QtObject model: null // GameModel.h

    implicitWidth: internal.rootItemWidth
    implicitHeight: internal.rootItemHeight

    ColumnLayout {
        id: mainLayout

        anchors {
            fill: parent
            topMargin: internal.defaultMargin
            bottomMargin: internal.defaultMargin
            leftMargin: internal.defaultMargin
            rightMargin: internal.defaultMargin
        }

        spacing: internal.defaultMargin

        TicTacToeGameConfig {
            id: playersConfig

            model: root.model.playerConfigModel
            gameStatus: root.model.gameStatus
        }

        TicTacToeGameButtons {
            Layout.alignment: Qt.AlignHCenter

            gameStatus: root.model.gameStatus

            onStartButtonSignal: {
                root.model.gameStatus = !root.model.gameStatus
            }

            onResetButtonSignal: {
                root.model.reset()
            }
        }


        TicTacToeField {
            id: gameBoard

            Layout.alignment: Qt.AlignHCenter

            model: root.model.gameBoardModel
        }

    }

    Rectangle {
        id: errorPopup

        implicitWidth: internal.errorPopupWidth
        implicitHeight: internal.errorPopupHeight

        anchors.centerIn: parent

        color: internal.backgroundColor
        radius: internal.defaultPopupRadius
        visible: internal.errorModel.showPopup

        border.color: internal.borderColor
        border.width: internal.borderWidth

        Text {
            anchors.fill: parent
            anchors.leftMargin: internal.defaultMargin
            anchors.rightMargin: internal.defaultMargin
            anchors.topMargin: internal.defaultMargin
            anchors.bottomMargin: closeButton.height

            color: internal.startTextColor
            font.pixelSize: 15
            wrapMode: Text.WordWrap

            text: internal.errorMessage()
        }

        Button {
            id: closeButton

            implicitWidth: 100
            implicitHeight: 50

            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.bottomMargin: internal.defaultMargin
            anchors.leftMargin: (parent.width - width) / 2

            Text {
                anchors.centerIn: parent

                color: internal.startTextColor
                font.pixelSize: 20

                text: "Close"
            }

            onClicked: {
                internal.errorModel.reset();
                internal.errorModel.showPopup = false;
            }
        }
    }

    Rectangle {
        id: winnerPopup

        implicitWidth: internal.winnerPopupWidth
        implicitHeight: internal.winnerPopupHeight

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: parent.width / 2 - width / 2
        anchors.bottomMargin: gameBoard.width / 2

        color: internal.backgroundColor
        radius: internal.defaultPopupRadius
        visible: !model.gameStatus && model.hasWinner

        border.color: internal.borderColor
        border.width: internal.borderWidth

        Text {
            anchors.centerIn: parent

            color: internal.startTextColor
            font.pixelSize: 20

            text: model.winnerName
        }
    }

    QtObject {
        id: internal

        property QtObject errorModel: root.model.errorHandlerModel // ErrorHandlerModel from GameModel.h

        readonly property int rootItemWidth: 720
        readonly property int rootItemHeight: 540

        readonly property color backgroundColor: "#444444"
        readonly property color borderColor: "green"
        readonly property color startTextColor: "#FFFFFF"

        readonly property int defaultMargin: 20
        readonly property int defaultPopupRadius: 10

        readonly property string buttonText: root.model.gameStatus ? "Stop and clear" : "Start Game"

        readonly property int winnerPopupWidth: 250
        readonly property int winnerPopupHeight: 100
        readonly property int errorPopupWidth: 300
        readonly property int errorPopupHeight: 200

        readonly property int borderWidth: 2

        function errorMessage() {
            if (!internal.errorModel.showPopup) {
                return "Ok";
            }

            if (!internal.errorModel) {
                return "Error model is undefine";
            }

            if (internal.errorModel.error === ErrorType.Invalid) {
                return "Undefine error type";
            }

            return internal._errorMessageImpl(internal.errorModel.error);
        }

        function _errorMessageImpl(errorType) {
            if (errorType === ErrorType.PlayerTypeAlreadyExist) {
                return qsTr("Player type already exist, choose another one");
            }

            if (errorType === ErrorType.FirstMarkIsCross) {
                return qsTr("First player mark is Cross X ");
            }

            if (errorType === ErrorType.PlayerNameAlreadyExist) {
                return qsTr("Use another name");
            }

            if (errorType === ErrorType.PlayerMarkAlreadyExist) {
                return qsTr("Use another mark");
            }

            return qsTr("Unregister error type");
        }

    }
}
