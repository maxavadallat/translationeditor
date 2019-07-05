import QtQuick 2.0

import customcomponents 1.0

MouseArea {
    id: busyRoot

    anchors.fill: parent

    opacity: mainController.busy ? 1.0 : 0.0

    Behavior on opacity { TEFade { } }
    visible: opacity > 0.0

    preventStealing: true

    onClicked: { }

    Connections {
        target: mainController

        onBusyTextChanged: {
            // Set Busy Text
            busyLabel.text = newText;
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "Black"
        opacity: 0.8
    }

    TEBusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
    }

    TELabel {
        id: busyLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: busyIndicator.bottom
        anchors.topMargin: 64
        text: "Busy..."
    }

    // ...

}
