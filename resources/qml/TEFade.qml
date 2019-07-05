import QtQuick 2.0

import "qrc:/resources/qml/style"

PropertyAnimation {
    id: fadeAnimRoot
    property: "opacity"
    duration: TEStyle.defaultDurationShort
}
