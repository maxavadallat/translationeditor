import QtQuick 2.0

import "qrc:/resources/qml/style"

MouseArea {
    id: popupRoot

    z: 1

    width: 0
    height: 0

    Behavior on width { TEFade { } }
    Behavior on height { TEFade { } }

    property alias model: itemsRepeater.model

    property alias popupItemDelegate: itemsRepeater.delegate

    property int itemsSpacing: 0
    property int itemsMaxWidth: TEStyle.defaultPopupItemWidth
    property int popupHeight: itemsRepeater.count * TEStyle.popupItemHeight + (itemsRepeater.count - 1) * itemsSpacing + TEStyle.defaultMargin * 2

    property int currentIndex: 0

    Behavior on opacity { TEFade { } }
    visible: opacity > 0.0

    state: "hidden"

    // Item Clicked Signal
    signal itemClicked(var index)
    signal popupDismissed();

    Component.onCompleted: {
        // Update Popup Width
        updatePopupWidth();
    }

//    onCurrentIndexChanged: {
//        console.log("TEPopup.onCurrentIndexChanged - currentIndex: " + currentIndex);

//        // ...
//    }

    onItemClicked: {
        // Hide
        hide();
    }

    Keys.onReleased: {
        switch (event.key) {
            case Qt.Key_Escape:
                // Dismiss
                popupRoot.dismiss();
            break;
        }
    }

    // Show
    function show(posX, posY) {
        // Set state
        popupRoot.state = "shown";

        // Set Position
        popupRoot.x = Math.min(posX - TEStyle.defaultRadius, parent.width - popupRoot.width);
        popupRoot.y = Math.min(posY - TEStyle.defaultRadius, parent.height - popupRoot.height);

        // ...
    }

    // Hide
    function hide() {
        // Set state
        popupRoot.state = "hidden";
    }

    // Dismiss
    function dismiss() {
        // Hide
        popupRoot.hide();
        // Emit Dismissed Signal
        popupRoot.popupDismissed();
    }

    // Update Popup Width
    function updatePopupWidth() {
        // Reset Items Max Width
        popupRoot.itemsMaxWidth = TEStyle.defaultPopupItemWidth;
        // Get Children Count
        var cCount = itemsColumn.children.length;
        // Iterate Through Children
        for (var i=0; i<cCount-1; i++) {
            // Adjust Items Max Width
            popupRoot.itemsMaxWidth = Math.max(popupRoot.itemsMaxWidth, itemsColumn.children[i].width);
        }

        // Iterate Through Children
        for (var j=0; j<cCount-1; j++) {
            // Adjust Items Width
            itemsColumn.children[j].width = popupRoot.itemsMaxWidth;
        }

        //console.log("TEPopup.updatePopupWidth - itemsMaxWidth: " + itemsMaxWidth);
    }

    // Set Item Text
    function setItemText(index, newText) {
        // Get Child Items Count
        var ciCount = itemsColumn.children.length;

        // Check Index
        if (index >= 0 && index < ciCount) {
            // Get Child Item
            var item = itemsColumn.children[index];
            // Set Title Text
            item.title = newText;
        }
    }

    // Background Rectangle
    Rectangle {
        anchors.fill: parent

        color: TEStyle.defaultColorBackground
        border.color: TEStyle.defaultColorBorder

        radius: TEStyle.defaultRadius
    }

    // Floating Item Highlight
    Rectangle {
        id: itemHighlight

        y: popupRoot.currentIndex * itemHighlight.height - itemsSpacing / 2 + TEStyle.defaultMargin

        Behavior on y { TEFade { } }

        width: parent.width
        height: TEStyle.popupItemHeight + itemsSpacing

        color: TEStyle.defaultColorListHighlight
    }

    // Items Column
    Column {
        id: itemsColumn

        anchors.top: parent.top
        anchors.topMargin: TEStyle.defaultMargin

        spacing: popupRoot.itemsSpacing

        add: Transition {
            ScriptAction {
                script: {
                    // Update Popup Width
                    updatePopupWidth();
                }
            }
        }

        // Items Repeater
        Repeater {
            id: itemsRepeater

            delegate: TEPopupItem {
                title: modelData

                onClicked: {
                    // Emit Item Clicked Signal
                    popupRoot.itemClicked(index);
                }

                onContainsMouseChanged: {
                    // Check Contains Mouse
                    if (containsMouse) {
                        // Set Current Index
                        popupRoot.currentIndex = index;
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "hidden"

            PropertyChanges { target: popupRoot; opacity: 0.0; width: 0; height: 0 }
        },

        State {
            name: "shown"

            PropertyChanges { target: popupRoot; currentIndex: 0 }
            PropertyChanges { target: popupRoot; opacity: 1.0; width: popupRoot.itemsMaxWidth; height: popupRoot.popupHeight }
            PropertyChanges { target: popupRoot; focus: true }
        }
    ]
}
