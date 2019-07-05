import QtQuick 2.0

import "qrc:/resources/qml/style"

MouseArea {
    id: buttonBaseRoot

    property bool checked: false

    property bool highlight: pressed

    opacity: enabled ? 1.0 : TEStyle.disabledOpacity
}
