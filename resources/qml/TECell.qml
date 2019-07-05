import QtQuick 2.0

import customcomponents 1.0

import "qrc:/resources/qml/style"

MouseArea {
    id: cellRoot

    height: TEStyle.defaultCellHeight

    property bool editable: false
    property int cellIndex: -1
    property bool cellWarning: false
    property bool cellCheckActive: false

    property alias text: cellTextLabel.text
    property alias horizontalAlignment: cellTextLabel.horizontalAlignment

    signal cellPressed(var cell, var modifiers)
    signal cellClicked(var cell, var modifiers)
    signal cellDoubleClicked(var cell)

    onPressed: {
        // Emit Cell Pressed Signal
        cellRoot.cellPressed(cellRoot, mouse.modifiers);
    }

    onClicked: {
        // Emit Cell Clicked
        cellRoot.cellClicked(cellRoot, mouse.modifiers);
    }

    onDoubleClicked: {
        // Emit Cell Double Clicked
        cellRoot.cellDoubleClicked(cellRoot);
    }

    // Background & Border
    Rectangle {
        id: cellBGRect
        anchors.fill: parent
        anchors.topMargin: 1
        anchors.bottomMargin: 1
        radius: TEStyle.defaultRadius

        opacity: 0.5

        color: cellRoot.cellWarning ? "#77FF0000" : "Transparent"

        border.color: {
            // Check Cell Check Active
            if (cellRoot.cellCheckActive) {
                return TEStyle.defaultColorSelect;
            }

            // Check Editable & Pressed
            if (cellRoot.editable && cellRoot.pressed) {
                return TEStyle.defaultColorButtonPressed;
            }

            // Check Edit Mode & Editable
            if (mainController.cellEditMode && cellRoot.editable) {

                return TEStyle.defaultColorBorder;
            }

            return "Transparent";
        }
    }

    // Cell Text
    TELabel {
        id: cellTextLabel
        anchors.fill: parent
        anchors.leftMargin: TEStyle.defaultMargin
        anchors.rightMargin: TEStyle.defaultMargin
        wrapMode: Text.NoWrap
        elide: Text.ElideLeft
        maximumLineCount: 1
    }

//    // Cell Border
//    Rectangle {
//        height: TEStyle.listItemHeight
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.verticalCenter: parent.verticalCenter

//        color: "Transparent"

//        border.color: {
//            if (cellMouseArea.pressed) {
//                return TEStyle.defaultColorButtonPressed;
//            }

//            if (cellRoot.cellCheckActive) {
//                return TEStyle.defaultColorSelect;
//            }

//            return TEStyle.defaultColorBorder
//        }

//        visible: (mainController.cellEditMode && cellRoot.editable) || cellRoot.cellCheckActive || cellMouseArea.pressed
//        opacity: 0.5
//        radius: TEStyle.defaultRadius
//    }

//    // Cell Mouse Area
//    MouseArea {
//        id: cellMouseArea
//        height: TEStyle.listItemHeight
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.verticalCenter: parent.verticalCenter
//        visible: cellRoot.editable

//        onPressed: {
//            // Emit Cell Pressed Signal
//            cellRoot.cellPressed(cellRoot, mouse.modifiers);
//        }

//        onClicked: {
//            // Emit Cell Clicked
//            cellRoot.cellClicked(cellRoot, mouse.modifiers);
//        }

//        onDoubleClicked: {
//            // Emit Cell Double Clicked
//            cellRoot.cellDoubleClicked(cellRoot);
//        }
//    }
}
