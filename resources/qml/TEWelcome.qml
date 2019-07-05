import QtQuick 2.0

import "qrc:/resources/qml/style"

Item {
    id: welcomeRoot

    anchors.fill: parent

    opacity: mainController.currentPackage ? 0.0 : 1.0
    Behavior on opacity { TEFade { } }
    visible: opacity > 0.0

    Column {
        anchors.centerIn: parent

        TELabel {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Welcome to Translation Editor"
            font.pixelSize: TEStyle.defaultFontSizeL
        }

        Item { width: 256; height: 8 }

        Rectangle { width: 300; height: 1; anchors.horizontalCenter: parent.horizontalCenter; color: TEStyle.defaultColorText; opacity: 0.7 }

        Item { width: 256; height: 8 }

        TELabel {
            //width: 256
            anchors.horizontalCenter: parent.horizontalCenter
            text: "- Create a New Project: ⌘N"
            font.pixelSize: TEStyle.defaultFontSizeL
        }

        TELabel {
            //width: 256
            anchors.horizontalCenter: parent.horizontalCenter
            text: "- Open Project: ⌘O"
            font.pixelSize: TEStyle.defaultFontSizeL
        }

        TELabel {
            //width: 256
            anchors.horizontalCenter: parent.horizontalCenter
            text: "- Import Files: ⌘I"
            font.pixelSize: TEStyle.defaultFontSizeL
        }

        Item { width: 256; height: 8  }

        Rectangle { width: 300; height: 1; anchors.horizontalCenter: parent.horizontalCenter; color: TEStyle.defaultColorText; opacity: 0.7 }

        Item { width: 256; height: 8; visible: rpListView.count > 0 }

        TELabel {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Recent Projects"
            visible: rpListView.count > 0
        }

        Item {
            width: 256
            height: 8
            visible: rpListView.count > 0
        }

        // Recent Project List
        ListView {
            id: rpListView
            width: welcomeRoot.width
            height: Math.min(contentHeight, delegateHeight * 5)

            property int delegateHeight: TEStyle.defaultFontSizeXL

            anchors.horizontalCenter: parent.horizontalCenter

            model: mainController.recentFiles

            delegate: MouseArea {
                id: delegateRoot

                width: rpListView.width
                height: rpListView.delegateHeight

                property bool hovering: false

                hoverEnabled: true

                TELabel {
                    id: titleText
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    elide: Text.ElideMiddle
                    horizontalAlignment: Text.AlignHCenter

                    color: {
                        if (delegateRoot.pressed)
                            return TEStyle.defaultColorSelect;

                        if (delegateRoot.hovering)
                            return TEStyle.defaultColorSelect;

                        return TEStyle.defaultColorText;
                    }

                    text: modelData
                }

                onEntered: {
                    delegateRoot.hovering = true;
                }

                onExited: {
                    delegateRoot.hovering = false;
                }

                onCanceled: {
                    delegateRoot.hovering = false;
                }

                onClicked: {
                    // Open Project
                    mainController.loadPackage(modelData);
                }
            }
        }
    }
/*
    Rectangle {
        x: 100
        y: 100

        width: 400
        height: 400

        border.color: "Lime"

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#AA010101" }
            GradientStop { position: 0.5; color: Qt.lighter("#AA010101", 32.0) }
            GradientStop { position: 1.0; color: "#AA010101" }
        }
    }
*/
}
