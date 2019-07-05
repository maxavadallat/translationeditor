import QtQuick 2.0

import "qrc:/resources/qml/style"

Text {
    id: labelRoot

    renderType: Text.NativeRendering
    color: TEStyle.defaultColorText
    font.family: preferences.guiFontName
    font.pixelSize: preferences.guiFontSize
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    elide: Text.ElideRight
    opacity: enabled ? 1.0 : TEStyle.disabledOpacity
    visible: opacity > 0.0
}
