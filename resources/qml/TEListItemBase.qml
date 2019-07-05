import QtQuick 2.0

import "qrc:/resources/qml/style"

MouseArea {
    id: listItemBaseRoot

    height: TEStyle.listItemHeight

    property bool highlight: pressed

    // ...
}
