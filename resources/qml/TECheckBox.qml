import QtQuick 2.0

import "qrc:/resources/qml/style"

TEButtonBase {
    id: checkBoxRoot

    width: TEStyle.buttonWidth
    height: TEStyle.buttonHeight

    property string buttonText: ""

    TELabel {
        id: textLabel
        anchors.right: checkBoxFrame.left
        anchors.rightMargin: TEStyle.defaultMargin * 2
        anchors.verticalCenter: parent.verticalCenter
        text: checkBoxRoot.buttonText
    }

    Rectangle {
        id: checkBoxFrame
        width: TEStyle.checkBoxSize
        height: TEStyle.checkBoxSize
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        border.color: TEStyle.defaultColorBorder
        color: "transparent"

        TEImage {
            anchors.centerIn: parent
            opacity: checkBoxRoot.checked ? 1.0 : 0.0
            Behavior on opacity { PropertyAnimation { duration: 100 } }
            source: preferences.guiTheme === 0 ? "qrc:/resources/images/icons/icon_check_black-16.png"
                                               : "qrc:/resources/images/icons/icon_check_white-16.png"
        }

//        TELabel {
//            anchors.centerIn: parent
//            text: "X"
//            opacity: checkBoxRoot.checked ? 1.0 : 0.0
//            Behavior on opacity { PropertyAnimation { duration: 100 } }
//        }
    }

    // ...
}
