import QtQuick 2.0

import "qrc:/resources/qml/style"

FocusScope {
    id: mainRoot

    focus: true

    // Main BG For Black Color
    Rectangle { anchors.fill: parent; color: "Black" }

    TEBackground {
        id: background
    }

    TEMainView {
        id: mainView
        //opacity: 0.0
    }

    TEWelcome {
        id: welcomeScreen
        //opacity: 1.0
    }

    TEBusyView {
        id: busyView
        //opacity: 1.0
    }
}
