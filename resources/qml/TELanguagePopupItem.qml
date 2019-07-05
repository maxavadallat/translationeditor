import QtQuick 2.0

import "qrc:/resources/qml/style"

MouseArea {
    id: itemRoot

    width: languageNameLabel.width + languageFlag.width + selectionCheckBox.width
    height: TEStyle.popupItemHeight

    hoverEnabled: true

    property alias source: languageFlag.source
    property alias text: languageNameLabel.text
    property alias checked: selectionCheckBox.checked

    TEImage {
        id: languageFlag

        width: height
        height: itemRoot.height - TEStyle.defaultMargin

        anchors.left: parent.left
        anchors.leftMargin: TEStyle.defaultMargin
        anchors.verticalCenter: parent.verticalCenter

        fillMode: Image.PreserveAspectFit
        source: itemRoot.source
    }

    TELabel {
        id: languageNameLabel

        anchors.left: languageFlag.right
        anchors.leftMargin: TEStyle.defaultMargin
        anchors.verticalCenter: parent.verticalCenter

        opacity: 1.0
        text: itemRoot.text
    }

    TECheckBox {
        id: selectionCheckBox

        anchors.right: parent.right
        anchors.rightMargin: TEStyle.defaultMargin
        anchors.verticalCenter: parent.verticalCenter

        propagateComposedEvents: true
        enabled: true
        checked: itemRoot.checked
    }
}
