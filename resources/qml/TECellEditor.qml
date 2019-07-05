import QtQuick 2.0
import QtGraphicalEffects 1.0

import "qrc:/resources/qml/style"

Rectangle {
    id: cellEditorRoot

    width: 420
    height: TEStyle.listItemHeight

    property alias text: cellEditor.text

    property int xAnimDuration: TEStyle.defaultDurationShort
    property int yAnimDuration: TEStyle.defaultDurationShort
    property int widthAnimDuration: TEStyle.defaultDurationShort

    Behavior on x {
        SequentialAnimation {
            NumberAnimation { duration: xAnimDuration }
            ScriptAction { script: { xAnimDuration = 0; } }
        }
    }

    Behavior on y {
        SequentialAnimation {
            NumberAnimation { duration: yAnimDuration }
            ScriptAction { script: { yAnimDuration = 0; } }
        }
    }

    Behavior on width {
        SequentialAnimation {
            NumberAnimation { duration: widthAnimDuration }
            ScriptAction { script: { widthAnimDuration = 0; } }
        }
    }

    property bool cellVisible: true

    property int lastCellIndex: -1

    state: "hidden"

    color: TEStyle.defaultColorHeader
    border.color: TEStyle.defaultColorBorder

    radius: TEStyle.defaultRadius

    Behavior on opacity { NumberAnimation { duration: 100 } }
    visible: opacity > 0.0

    layer.enabled: true

    layer.effect: Glow {
        radius: TEStyle.defaultRadius * 2
        samples: 17
        color: Qt.lighter(cellEditorRoot.border.color)
    }

    signal accepted()
    signal canceled()
    signal editorHidden()
    signal cleared();

    signal downClicked()
    signal upClicked()
    signal nextClicked();
    signal prevClicked();

    onCellVisibleChanged: {
        // Check Cell Visibility
        if (!cellVisible) {
            // Hide
            hide();
        }
    }

    onStateChanged: {
        // Check State
        if (state === "shown") {
            // Request Focus
            cellEditor.focus = true;
        }
    }

    // Show
    function show() {
        // Set State
        cellEditorRoot.state = "shown";
    }

    // Hide
    function hide() {
        // Set State
        cellEditorRoot.state = "hidden";
        // Emit Editor Hidden Signal
        cellEditorRoot.editorHidden();
    }

    // Set Text
    function setText(text) {
        // Set Text
        cellEditor.text = text;
        // Set Cursor Pos
        //cellEditor.cursorPosition = text.length;
        // Select All
        cellEditor.selectAll();
        // Set Focus
        cellEditor.forceActiveFocus();
    }

    // Filter Editor
    TextInput {
        id: cellEditor

        anchors.left: parent.left
        anchors.leftMargin: TEStyle.defaultMargin
        anchors.right: clearButton.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 0

        property bool hadSelection: false
        property bool canMovetoOther: false

        font.family: preferences.guiFontName
        font.pixelSize: preferences.guiFontSize

        verticalAlignment: TextInput.AlignVCenter
        color: TEStyle.defaultColorText

        clip: true

        Keys.onPressed: {
            switch (event.key) {
                case Qt.Key_Left:
//                    console.log("TECellEditor.cellEditor.Keys.onPressed - LEFT - selectedText: " + cellEditor.selectedText);
//                    console.log("TECellEditor.cellEditor.Keys.onPressed - LEFT - selectionStart: " + cellEditor.selectionStart);
//                    console.log("TECellEditor.cellEditor.Keys.onPressed - LEFT - selectionEnd: " + cellEditor.selectionEnd);

                    // Check Selected Text Length
                    if (cellEditor.selectedText.length > 0) {
                        // Set Had Selection
                        hadSelection = true;
                    } else if (cellEditor.cursorPosition === 0) {
                        // Set Can Move to other
                        canMovetoOther = true;
                    }

                break;

                case Qt.Key_Right:
//                    console.log("TECellEditor.cellEditor.Keys.onPressed - RIGHT - selectedText: " + cellEditor.selectedText);
//                    console.log("TECellEditor.cellEditor.Keys.onPressed - RIGHT - selectionStart: " + cellEditor.selectionStart);
//                    console.log("TECellEditor.cellEditor.Keys.onPressed - RIGHT - selectionEnd: " + cellEditor.selectionEnd);

                    // Check Selected Text Length
                    if (cellEditor.selectedText.length > 0) {
                        // Set Had Selection
                        hadSelection = true;
                    } else if (cellEditor.cursorPosition === cellEditor.length) {
                        // Set Can Move to other
                        canMovetoOther = true;
                    }

                break;
            }
        }

        Keys.onReleased: {
            switch (event.key) {
                case Qt.Key_Escape:
                    // Emit Canceled Signal
                    cellEditorRoot.canceled();
                    // Hide Filter Editor
                    cellEditorRoot.hide();
                break;

                case Qt.Key_Return:
                    // Emit Accepted Signal
                    cellEditorRoot.accepted();
                break;

                case Qt.Key_Left:
                    // Check Had Selection
                    if (hadSelection) {
                        // Reset HAd Selection
                        hadSelection = false;
                    } else if (canMovetoOther) {
                        // Reset Can Move To Other
                        canMovetoOther = false;
                        // Emit Prev Clicked Signal
                        cellEditorRoot.prevClicked();
                    }
                break;

                case Qt.Key_Right:
                    // Check Had Selection
                    if (hadSelection) {
                        // Reset HAd Selection
                        hadSelection = false;
                    } else if (canMovetoOther) {
                        // Reset Can Move To Other
                        canMovetoOther = false;
                        // Emit Next Clicked Signal
                        cellEditorRoot.nextClicked();
                    }
                break;

                case Qt.Key_Up:
                    // Emit Up Clicked Signal
                    cellEditorRoot.upClicked();
                break;

                case Qt.Key_Down:
                    // Emit Down Clicked Signal
                    cellEditorRoot.downClicked();
                break;

                case Qt.Key_Tab:
                    // Check Modifiers
                    if (event.modifiers & Qt.ControlModifier) {
                        // Emit Prev Clicked Signal
                        cellEditorRoot.prevClicked();
                    } else {
                        // Emit Next Clicked Signal
                        cellEditorRoot.nextClicked();
                    }
                break;

                default:

                break;
            }
        }
    }

    // Clear Button
    TEImageButton {
        id: clearButton

        width: (cellEditor.text.length > 0) ? TEStyle.buttonHeight * 0.8 : 0
        Behavior on width { TEFade { property: "width" } }

        anchors.right: parent.right
        anchors.rightMargin: TEStyle.defaultMargin * 0.5
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 1

        opacity: cellEditor.text.length > 0 ? 1.0 : 0.0
        Behavior on opacity { TEFade { } }
        visible: opacity > 0

        iconSouorce: "qrc:/resources/images/icons/icon_clear-16.png"

        onClicked: {
            // Reset Editor Text
            cellEditor.text = "";
            // Emit Clearerd Signal
            cellEditorRoot.cleared();
        }
    }

    states: [
        State {
            name: "shown"

            PropertyChanges { target: cellEditorRoot; opacity: 1.0 }
        },

        State {
            name: "hidden"

            PropertyChanges { target: cellEditorRoot; opacity: 0.0 }
        }
    ]
}
