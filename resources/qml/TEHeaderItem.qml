import QtQuick 2.0

import "qrc:/resources/qml/style"

MouseArea {
    id: headerItemRoot

    height: TEStyle.defaultHeaderHeight

    property int headerItemIndex: -1

    property string language: ""
    property string title: "Title"

    property int minWidth: 56
    property int maxWidth: -1

    property alias textAlignment: titleLabel.horizontalAlignment

    property bool enableResizeHandle: true
    property bool clipText: true
    property bool storeSize: true

    property int sortingColumn: -1
    property bool sortDescending: false

    property Connections preferencesConnections: Connections {
        target: preferences

        onDefaultsLoaded: {
            // Set Width
            headerItemRoot.width = preferences.getColumnWidth(headerItemIndex);
        }

//        onColumnWidthChanged: {
//            // Check If Resize Handle Mouse Area Pressed
//            if (!resizeHandleMouseArea.pressed && headerItemIndex === aIndex) {
//                console.log("TEHeaderItem.preferencesConnections.onColumnWidthChanged - aIndex: " + aIndex + " - aWidth: " + aWidth);
//            }

//            // ...
//        }
    }

    onWidthChanged: {
        // Check If Resize Handle Mouse Area Pressed
        if (storeSize && resizeHandleMouseArea.pressed) {
            // Set Column Width
            preferences.setColumnWidth(headerItemRoot.headerItemIndex, headerItemRoot.width);
        }
    }

    // Background
    Rectangle {
        id: background
        anchors.fill: parent
        color: parent.pressed ? Qt.lighter(TEStyle.defaultColorHeader) : TEStyle.defaultColorHeader
        clip: headerItemRoot.clipText

        // Title Label
        TELabel {
            id: titleLabel
            anchors.left: parent.left
            anchors.leftMargin: TEStyle.defaultMargin / 2
            anchors.right: parent.right
            anchors.rightMargin: TEStyle.defaultMargin / 2
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignLeft
            wrapMode: Text.NoWrap
            clip: true
            text: headerItemRoot.title
        }

        TEImage {
            anchors.right: parent.right
            anchors.rightMargin: TEStyle.defaultMargin
            anchors.verticalCenter: parent.verticalCenter

            scale: 0.7

            visible: headerItemRoot.sortingColumn === headerItemRoot.headerItemIndex

            source: headerItemRoot.sortDescending ? "qrc:/resources/images/core/arrowhead-down-16.png"
                                                  : "qrc:/resources/images/core/arrowhead-up-16.png"
        }
    }

    // Resize Grab Area
    MouseArea {
        id: resizeHandleMouseArea

        x: parent.width

        width: TEStyle.grabAreaSize
        height: parent.height

        cursorShape: Qt.SizeHorCursor

        enabled: headerItemRoot.enableResizeHandle

        property int pressedSize: 0
        property int pressX: 0

        //drag.active: pressed()
        drag.target: resizeHandleMouseArea
        drag.axis: Drag.XAxis
        drag.threshold: TEStyle.defaultSpacing
        drag.minimumX: headerItemRoot.minWidth

        onPressed: {
            // Set Position - Break
            x = parent.width;
        }

        onXChanged: {
            // Check Pressed State
            if (pressed) {
                // Set Parent Width
                parent.width = x;
            }
        }

        onReleased: {
            // Set Position - Bind
            x = Qt.binding(function() { return parent.width; });
        }

        Rectangle {
            anchors.fill: parent
            color: "orange"
            opacity: parent.pressed ? 1.0 : 0.4
        }
    }
}
