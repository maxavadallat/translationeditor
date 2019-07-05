import QtQuick 2.0

import "qrc:/resources/qml/style"

TEButtonBase {
    id: imageButtonRoot

    width: TEStyle.buttonWidth
    height: TEStyle.buttonHeight

    property string iconSouorce: ""

    TEImage {
        id: buttonImage
        anchors.fill: parent
        source: imageButtonRoot.iconSouorce
    }
}
