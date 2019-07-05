import QtQuick 2.0
import QtGraphicalEffects 1.0

import "qrc:/resources/qml/style"

Item {
    id: filterEditorRoot

    width: parent.width
    height: TEStyle.listItemHeight * 1.5

    anchors.bottom: parent.bottom
    anchors.bottomMargin: editorBottomMargin

    property alias filterText: filterEditor.text

    property int editorBottomMargin: 0
    Behavior on editorBottomMargin { PropertyAnimation { duration: TEStyle.defaultDurationNormal } }

    property alias showChecked: showCheckedCheckbox.checked
    property alias showDraft: showDraftCheckBox.checked
    property alias showWarning: showWarningCheckBox.checked

    state: "hidden"

    signal showCheckedClicked()
    signal showDraftClicked()
    signal showWarningClicked()

    // Show
    function show() {
        // Set State
        filterEditorRoot.state = "shown";
    }

    // Hide
    function hide() {
        // Set State
        filterEditorRoot.state = "hidden";
    }

    onStateChanged: {
        // Check State
        if (state === "shown") {
            // Request Focus
            filterEditor.focus = true;
        }
    }

    // Main Controller Connections
    Connections {
        target: mainController

        // Toggle Filter Editor
        onToggleFilterEditor: {
            // Check Current Package
            if (mainController.currentPackage) {
                // Check State
                if (filterEditorRoot.state === "hidden") {
                    // Show
                    show();
                } else {
                    // Hide
                    hide();
                }
            }
        }
    }

    // Background
    Rectangle {
        anchors.fill: parent
        color: TEStyle.defaultColorBackground
//        border.color: "Lime"
    }

    Row {
        id: filterCheckRow

        anchors.right: editFilterTitleLabel.left
        anchors.rightMargin: TEStyle.defaultMargin * 8
        anchors.verticalCenter: parent.verticalCenter

        spacing: TEStyle.defaultSpacing * 4

        TECheckBox {
            id: showCheckedCheckbox
            width: 148
            buttonText: "Show Checked"
            onClicked: filterEditorRoot.showCheckedClicked();
        }

        TECheckBox {
            id: showDraftCheckBox
            width: 148
            buttonText: "Show Draft"
            onClicked: filterEditorRoot.showDraftClicked();
        }

        TECheckBox {
            id: showWarningCheckBox
            width: 148
            buttonText: "Show Warning"
            onClicked: filterEditorRoot.showWarningClicked();
        }
    }


    // Edit Filter Title Label
    TELabel {
        id: editFilterTitleLabel
        anchors.right: editorBorder.left
        anchors.rightMargin: TEStyle.defaultMargin
        anchors.verticalCenter: parent.verticalCenter
        text: "Edit Filter: "
    }

    // Border Rect
    Rectangle {
        id: editorBorder

        width: 320
        height: TEStyle.listItemHeight

        anchors.right: parent.right
        anchors.rightMargin: TEStyle.defaultMargin * 2

        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -1

        //color: TEStyle.defaultColorButton
        color: "DarkGray"
        border.color: TEStyle.defaultColorBorder

        radius: TEStyle.defaultRadius

        layer.enabled: true
        layer.effect: InnerShadow {
            horizontalOffset: 2
            verticalOffset: 2
            radius: TEStyle.defaultRadius
            samples: 15
            color: Qt.darker(editorBorder.color)
        }

        // Filter Editor
        TextInput {
            id: filterEditor

            anchors.left: parent.left
            anchors.leftMargin: TEStyle.defaultMargin
            anchors.right: clearButton.left
            anchors.top: parent.top
            anchors.topMargin: TEStyle.defaultMargin

            //font.family: TEStyle.fontLoader.name
            font.pixelSize: TEStyle.defaultFontSizeL
            verticalAlignment: TextInput.AlignVCenter
            color: TEStyle.defaultColorText

            clip: true

            Keys.onReleased: {
                switch (event.key) {
                    case Qt.Key_Escape:
                        // Clear Filter
                        filterEditor.text = "";
                        // Hide Filter Editor
                        filterEditorRoot.hide();
                    break;

                    default:
                    break;
                }
            }
        }

        // Clear Button
        TEImageButton {
            id: clearButton

            width: (filterEditor.text.length > 0) ? TEStyle.buttonHeight * 0.8 : 0
            Behavior on width { TEFade { property: "width" } }

            anchors.right: parent.right
            anchors.rightMargin: TEStyle.defaultMargin * 0.5
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 1

            opacity: filterEditor.text.length > 0 ? 1.0 : 0.0
            Behavior on opacity { TEFade { } }
            visible: opacity > 0

            iconSouorce: "qrc:/resources/images/icons/icon_clear-16.png"

            onClicked: {
                // Reset Editor Text
                filterEditor.text = "";
                // Set Focus
                filterEditor.focus = true;
            }
        }
    }

    states: [
        State {
            name: "shown"

            PropertyChanges { target: filterEditorRoot; editorBottomMargin: 0 }
        },

        State {
            name: "hidden"

            PropertyChanges { target: filterEditorRoot; editorBottomMargin: -filterEditorRoot.height }
        }
    ]
}
