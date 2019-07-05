import QtQuick 2.0

import "qrc:/resources/qml/style"

Rectangle {
    id: backgroundRoot

    anchors.fill: parent

    gradient: Gradient {
        GradientStop { position: 0.0; color: TEStyle.defaultColorGradient1 }
        GradientStop { position: 0.5; color: TEStyle.defaultColorGradient2 }
        GradientStop { position: 1.0; color: TEStyle.defaultColorGradient1 }
    }
}
