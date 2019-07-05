import QtQuick 2.0

import "qrc:/resources/qml/style"

TEButtonBase {
    id: buttonRoot

    width: TEStyle.buttonWidth
    height: TEStyle.buttonHeight

    property string text: "Button"

    property string defaultColor: TEStyle.defaultColorButton
    property string defaultColorPressed: TEStyle.defaultColorButtonPressed
    property string defaultColorChecked: TEStyle.defaultColorButtonChecked

    clip: true

    Rectangle {
        anchors.fill: parent

        radius: TEStyle.defaultRadius

        color: {
            // Check Highlight
            if (buttonRoot.highlight) {
                return defaultColorPressed;
            }

            // Check Checked State
            if (buttonRoot.checked) {
                return defaultColorChecked;
            }

            return defaultColor;
        }
    }

    TELabel {
        anchors.centerIn: parent
        text: buttonRoot.text
    }

    // ...
}
