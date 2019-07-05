import QtQuick 2.0

import "qrc:/resources/qml/style"

MouseArea {
    id: popupItemRoot

    width: {
        if (title === "-") {
            return TEStyle.defaultPopupItemWidth;
        }

        return textLabel.width + TEStyle.defaultMargin * 2
    }

    height: TEStyle.popupItemHeight

    property string title: "Item"

    hoverEnabled: title !== "-"

    TELabel {
        id: textLabel
        anchors.left: parent.left
        anchors.leftMargin: TEStyle.defaultMargin
        anchors.verticalCenter: parent.verticalCenter
        text: popupItemRoot.title

        visible: title !== "-"
    }

    Rectangle {
        id: separator

        height: 1

        anchors.left: parent.left
        //anchors.leftMargin: TEStyle.defaultMargin
        anchors.right: parent.right
        //anchors.rightMargin: TEStyle.defaultMargin
        anchors.verticalCenter: parent.verticalCenter

        color: TEStyle.defaultColorBorder

        visible: title === "-"
    }
}
