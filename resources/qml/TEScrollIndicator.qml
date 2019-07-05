import QtQuick 2.0

import "qrc:/resources/qml/style"

Item {
    id: scrollIndicatorRoot

    width: vertical ? TEStyle.scrollIndicatorWidth : viewSize
    height: vertical ? viewSize : TEStyle.scrollIndicatorWidth

    property bool vertical: true

    property real viewSize: 0
    property real viewContentSize: 0
    property real viewContentPos: 0

    opacity: 0.0
    Behavior on opacity { TEFade { } }
    visible: opacity > 0.0

    onViewContentPosChanged: {
        // Check View & Content Size
        if (viewContentSize > viewSize) {
            // Set Opacity
            opacity = 1.0;
            // Restart Idle Timer
            idleTimer.restart();
        }
    }

    // Background Rectangle
    Rectangle {
        id: background
        anchors.fill: parent
        radius: TEStyle.scrollIndicatorWidth / 2
        border.width: 1
        border.color: TEStyle.defaultColorBorder
        color: "transparent"
        smooth: true
        clip: true

        // Indicator
        Rectangle {
            id: indicator

            x: scrollIndicatorRoot.vertical ? 0 : (scrollIndicatorRoot.viewSize - width) * scrollIndicatorRoot.viewContentPos /
                                                  (scrollIndicatorRoot.viewContentSize - scrollIndicatorRoot.viewSize)

            y: scrollIndicatorRoot.vertical ? (scrollIndicatorRoot.viewSize - height) * scrollIndicatorRoot.viewContentPos  /
                                               (scrollIndicatorRoot.viewContentSize - scrollIndicatorRoot.viewSize) : 0

            width: {
                if (vertical) {
                    return parent.width;
                }

                if (scrollIndicatorRoot.viewContentSize <= 0) {
                    return 0;
                }

                return (scrollIndicatorRoot.viewSize * scrollIndicatorRoot.viewSize) / scrollIndicatorRoot.viewContentSize;
            }

            height: {
                if (!vertical) {
                    return parent.height;
                }

                if (scrollIndicatorRoot.viewContentSize <= 0) {
                    return 0;
                }

                return (scrollIndicatorRoot.viewSize * scrollIndicatorRoot.viewSize) / scrollIndicatorRoot.viewContentSize;
            }

            radius: TEStyle.scrollIndicatorWidth / 2
            color: TEStyle.defaultColorBorder
            border.color: Qt.lighter(color)
            smooth: true
        }
    }

    Timer {
        id: idleTimer
        repeat: false
        interval: TEStyle.defaultDurationLong * 6
        onTriggered: {
            // Reset Opacity
            scrollIndicatorRoot.opacity = 0.0;
        }
    }
}
