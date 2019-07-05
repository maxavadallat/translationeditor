import QtQuick 2.0

Item {
    id: busyIndicatorRoot

    width: 128
    height: 128

    readonly property int count: 12
    readonly property int step: 360 / count

    Repeater {
        model: busyIndicatorRoot.count

        Item {
            width: busyIndicatorRoot.width
            height: 4
            anchors.centerIn: busyIndicatorRoot
            rotation: index * busyIndicatorRoot.step + busyIndicatorRoot.step

            Rectangle {
                width: parent.width * 0.25
                height: parent.height
                anchors.right: parent.right
                anchors.rightMargin: radius / 2
                color: "Gray"
                smooth: true
                radius: height / 2
                opacity: index / busyIndicatorRoot.count
            }
        }
    }

    Timer {
        running: busyIndicatorRoot.visible
        repeat: true
        interval: 100
        onTriggered: {
            parent.rotation = (parent.rotation + busyIndicatorRoot.step) % 360
        }
    }
}
