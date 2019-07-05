import QtQuick 2.0
import QtGraphicalEffects 1.0

import "qrc:/resources/qml/style"

TextInput {
    id: textBoxRoot

    // ...

    layer.enabled: activeFocus
    layer.effect: Glow {
        samples: 15
        color: TEStyle.defaultColorGlow
        transparentBorder: true
    }
}
