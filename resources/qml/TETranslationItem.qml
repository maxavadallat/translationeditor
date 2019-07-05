import QtQuick 2.0
//import QtGraphicalEffects 1.0

import "qrc:/resources/qml/style"

TEListItemBase {
    id: translationItemRoot

//    y: -1

    //z: isCurrent ? 0.1 : 0.0

    //height: TEStyle.listItemHeight * (isCurrent ? 1.5 : 1.0)
    //Behavior on height { PropertyAnimation { duration: 2000 } }

    property QtObject translationItem: null

    property int sourceIndex: -1

    property string textLabelID: "Label ID"
    property string textScreen: "Screen"
    property string textType: "Type"
    property string textState: "State"
    property string textOriginal: "Original Text"
    property string textTranslations: "Translations"

//    property bool isCurrent: false
    property bool selected: false

    property bool moving: false

    property bool editMode: false
    property int listContainerWidth: 0

    property bool itemWarning: false

    property int columnWidth1: preferences.getColumnWidth(0)
    property int columnWidth2: preferences.getColumnWidth(1)
    property int columnWidth3: preferences.getColumnWidth(2)
    property int columnWidth4: preferences.getColumnWidth(3)
    property int columnWidth5: preferences.getColumnWidth(4)

//    property alias itemCells: itemRow.children

    property Connections preferencesConnections: Connections {
        target: preferences

        onDefaultsLoaded: {
            // Update Column Widths
            updateColumnWidths();
        }

        onColumnWidthChanged: {
            //console.log("TETranslationItem.preferencesConnections.onColumnWidthChanged - aIndex: " + aIndex + " - aWidth: " + aWidth);

            // Switch Index
            switch (aIndex) {
                case 0: translationItemRoot.columnWidth1 = aWidth; break;
                case 1: translationItemRoot.columnWidth2 = aWidth; break;
                case 2: translationItemRoot.columnWidth3 = aWidth; break;
                case 3: translationItemRoot.columnWidth4 = aWidth; break;
                case 4: translationItemRoot.columnWidth5 = aWidth; break;

                default: {
/*
                    // Get Label
                    var translationLabel = itemRow.children[aIndex];
                    // Check Label
                    if (translationLabel) {
                        // Set Width
                        translationLabel.width = aWidth;
                    }
*/
                    // Get Translation Cell
                    var trCell = translationCellsLoader.active ? translationCellsLoader.item.children[aIndex - 5] : null;

                    // Check Translation Cell
                    if (trCell) {
                        // Set Width
                        trCell.width = aWidth;
                    }

                } break;
            }
        }
    }

    property bool alternateBG: false

    property bool translationCellsLoaded: translationCellsLoader.active

    acceptedButtons: Qt.LeftButton | Qt.RightButton

//    layer.enabled: translationItemRoot.isCurrent
//    layer.effect: DropShadow {
//        radius: TEStyle.defaultRadius
//        samples: 16
//        horizontalOffset: TEStyle.defaultSpacing
//        verticalOffset: TEStyle.defaultSpacing
//        spread: 0
//        transparentBorder: true
//    }

    signal delegateCreated(var itemInstance)
    signal delegateDestory(var itemInstance)

    signal editModeFinished()
    signal itemDelete(var deleteIndex)
//    signal itemDestroy()

    signal cellPressed(var cell, var cellIndex, var modifiers)
    signal cellClicked(var cell, var cellIndex, var modifiers)
    signal cellDoubleClicked(var cell, var cellIndex)

    Component.onCompleted: {
        //console.log(">>>> TETranslationItem.onCompleted - sourceIndex: " + sourceIndex);

        // Check Moving State
        //if (!translationItemRoot.moving)
        {
            // Activate Translation Cells Loader
            //translationCellsLoader.activate();
        }

        // Set Delete Mode Loader Active State
        deleteModeLoader.active = true;

        // Emit Delegate Created Signal
        translationItemRoot.delegateCreated(translationItemRoot);
    }

    Component.onDestruction: {
        //console.log("<<<< TETranslationItem.onDestruction - sourceIndex: " + sourceIndex);

        // Emit Delegate Destroy Signal
        translationItemRoot.delegateDestory(translationItemRoot);

        // Emit Item Destroy Signal
        //translationItemRoot.itemDestroy();
    }

//    onMovingChanged: {
//        // Check Moving
//        if (!translationItemRoot.moving && !translationCellsLoader.active) {
//            // Activate Translation Cells Loader
//            translationCellsLoader.activate();
//        }
//    }

//    onYChanged: {
//        console.log("TETranslationItem.onYChanged - y: " + translationItemRoot.y);

//        // Load Translation Cells
//        loadTranslationCells();

//        // ...
//    }

    // Get Cell
    function getCell(cellIndex) {
        switch (cellIndex) {
            case 0: return textIDCell;
            case 1: return textScreenCell;
            case 2: return originalTextCell;

            default:
                // Check If Translation Cells Loader Active
                if (translationCellsLoader.active) {
                    return translationCellsLoader.item.children[cellIndex - 3];
                }
            break;
        }

        return null;
    }

    // Get Cell Pos X
    function getCellPosX(cellIndex) {
        switch (cellIndex) {
            case 0: return textIDCell.x;
            case 1: return textScreenCell.x;
            case 2: return originalTextCell.x;

            default:
                // Check If Translation Cells Loader Active
                if (translationCellsLoader.active) {
                    return translationCellsLoader.item.children[cellIndex - 3].x + translationCellsLoader.x;
                }
            break;
        }

        return 0;
    }

    // Get Next Cell Index
    function getNextCellIndex(index) {
        // Check Cell Edit Mode
        if (mainController.cellEditMode) {

            console.log("TETranslationItem.getNextCellIndex - index: " + index);

            switch (index) {
                case 0: return 1;
                case 1: return 2;

                default:
                    // Get Translation Cells Count
                    var trcCount = translationCellsLoader.active ? translationCellsLoader.item.children.length : 0;
                    // Init Cell Index
                    var cIndex = index - 2;
                    // Iterate Through Children
                    while (cIndex < trcCount - 2) {
                        // Get Child/Cell
                        var cell = translationCellsLoader.item.children[cIndex];

                        //console.log("cell: " + cell);

                        // Check Cell
                        if (cell && cell.visible && cell.cellIndex > index) {
                            return cell.cellIndex;
                        }


                        // Inc Child/Cell Index
                        cIndex++;
                    }
                break;
            }

            return 0;
        }

        return -1;
    }

    // Get Previous Cell Index
    function getPrevCellIndex(index) {
        // Check Cell Edit Mode
        if (mainController.cellEditMode) {
            console.log("TETranslationItem.getPrevCellIndex - index: " + index);

            switch (index) {
                case 3:
                case 2:
                case 1: return index - 1;

                default:
                    // Get Translation Cells Count
                    var trcCount = translationCellsLoader.active ? translationCellsLoader.item.children.length : 0;
                    // Init Cell Index
                    var cIndex = trcCount - 2;
                    // Iterate Through Children
                    while (cIndex > 0) {
                        // Get Cell
                        var cell = translationCellsLoader.item.children[cIndex];

                        //console.log("cell: " + cell);

                        // Check Cell
                        if (cell && cell.visible) {
                            // Check Index
                            if ((index === 0) || ((cell.cellIndex >= 0) && (cell.cellIndex < index))) {
                                return cell.cellIndex;
                            }
                        }

                        // Dec Child/Cell Index
                        cIndex--;
                    }

                return 2;
            }
        }

        return -1;
    }

    // Update Column Widths
    function updateColumnWidths() {
        // Set Column Widths
        translationItemRoot.columnWidth1 = preferences.getColumnWidth(0);
        translationItemRoot.columnWidth2 = preferences.getColumnWidth(1);
        translationItemRoot.columnWidth3 = preferences.getColumnWidth(2);
        translationItemRoot.columnWidth4 = preferences.getColumnWidth(3);
        translationItemRoot.columnWidth5 = preferences.getColumnWidth(4);

        // Get Translation Cells Row Children Count
        var tcrCount = translationCellsLoader.active ? translationCellsLoader.item.children.length : 0;

        // Iterate Through Children
        for (var i=0; i<tcrCount-1; i++) {
            // Get Children
            var child = translationCellsLoader.item.children[i];
            // Set Width
            child.width = preferences.getColumnWidth(i + 5);
        }
    }

    // Load Translation Cells
    function loadTranslationCells() {
        // Check If Translations Cells Are Loaded
        if (!translationItemRoot.translationCellsLoaded) {
            // Activate Translation Cells Loader
            translationCellsLoader.activate();
        }
    }

    // Background
    Rectangle {
        id: backgroundRect
        anchors.fill: parent
        color: {
            // Check Selected State
            if (translationItemRoot.selected) {
//                // Check If Current
//                if (translationItemRoot.isCurrent) {
//                    return Qt.lighter(TEStyle.defaultColorRowSelected);
//                }

                return TEStyle.defaultColorRowSelected;
            }

            return translationItemRoot.alternateBG ? TEStyle.defaultColorRowAlternate : "Transparent";
        }
    }

    // Item Row
    Row {
        id: itemRow
        anchors.left: parent.left
        anchors.leftMargin: TEStyle.defaultMargin
        anchors.verticalCenter: parent.verticalCenter

        spacing: TEStyle.defaultSpacing

        // Index
        TECell {
            id: indexCell
            width: translationItemRoot.columnWidth1
            anchors.verticalCenter: parent.verticalCenter
            //wrapMode: Text.NoWrap
            text: translationItemRoot.sourceIndex + 1

            onCellPressed: translationItemRoot.cellPressed(cell, cellIndex, modifiers);
            onCellClicked: translationItemRoot.cellClicked(cell, cellIndex, modifiers);
            onCellDoubleClicked: translationItemRoot.cellDoubleClicked(cell, cellIndex);
        }

        // Translation Text/Label ID
        TECell {
            id: textIDCell
            width: translationItemRoot.columnWidth2
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignLeft
            //wrapMode: Text.NoWrap

            editable: true
            cellIndex: 0

            text: translationItemRoot.textLabelID

            onCellPressed: translationItemRoot.cellPressed(cell, cellIndex, modifiers);
            onCellClicked: translationItemRoot.cellClicked(cell, cellIndex, modifiers);
            onCellDoubleClicked: translationItemRoot.cellDoubleClicked(cell, cellIndex);
        }

        // Translation Text Screen
        TECell {
            id: textScreenCell
            width: translationItemRoot.columnWidth3
            anchors.verticalCenter: parent.verticalCenter
            //wrapMode: Text.NoWrap

            editable: true
            cellIndex: 1

            text: translationItemRoot.textScreen

            onCellPressed: translationItemRoot.cellPressed(cell, cellIndex, modifiers);
            onCellClicked: translationItemRoot.cellClicked(cell, cellIndex, modifiers);
            onCellDoubleClicked: translationItemRoot.cellDoubleClicked(cell, cellIndex);
        }

        // State Cell
        Item {
            id: stateContainer

            width: translationItemRoot.columnWidth4
            height: translationItemRoot.height

            clip: true

            // Background
            Rectangle {
                width: parent.width - TEStyle.defaultMargin * 2
                height: parent.height - TEStyle.defaultMargin
                anchors.centerIn: parent

                radius: TEStyle.defaultRadius

                color: {
                    switch (translationItemRoot.textState) {
                        default:            return "Gray";
                        case "checking":
                        case "Checking":    return "Orange";
                        case "checked":
                        case "Checked":     return "Green";
                        case "warning":
                        case "Warning":     return "Red";
                    }
                }
            }

            // Translation State
            TELabel {
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                wrapMode: Text.NoWrap

                text: {
                    // Check If Checking Active
                    if (mainController.checkingActive && mainController.checkIndex === translationItemRoot.sourceIndex) {
                        return "Checking";
                    }

                    return translationItemRoot.textState;
                }

                color: "Snow" // Override Theme Color
            }
        }

        // Translation Original Text
        TECell {
            id: originalTextCell
            width: translationItemRoot.columnWidth5
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignLeft
            //wrapMode: Text.NoWrap
            //maximumLineCount: 1

            editable: true
            cellIndex: 2

            text: translationItemRoot.textOriginal

            onCellPressed: translationItemRoot.cellPressed(cell, cellIndex, modifiers);
            onCellClicked: translationItemRoot.cellClicked(cell, cellIndex, modifiers);
            onCellDoubleClicked: translationItemRoot.cellDoubleClicked(cell, cellIndex);
        }

//        // Cell Translations Load Delay timer
//        Timer {
//            id: cellTranslationsLoadDelayTimer
//            repeat: false
//            interval: TEStyle.defaultDurationShort
//            onTriggered: {
//                // Activate Translation Cells Loader
//                translationCellsLoader.activate();
//            }
//        }

        // Translation Cells Loader
        TELoader {
            id: translationCellsLoader
            height: parent.height

//            source: "qrc:/resources/qml/TETranslationCells.qml"

            sourceComponent: Row {
                id: translationCellsRoot

                height: translationCellsLoader.height

                spacing: TEStyle.defaultSpacing

                // Translation Cells Repeater
                Repeater {
                    id: translationsRepeater

                    model: translationItemRoot.textTranslations.split("\u001E")

                    // Translation Cells Delegate
                    delegate: TECell {
                        id: cellDelegateRoot

                        width: preferences.getColumnWidth(index + 5)
                        anchors.verticalCenter: parent.verticalCenter

                        horizontalAlignment: Text.AlignJustify

                        editable: true
                        cellIndex: 3 + index

                        text: modelData

                        property int languageIndex: index

                        property string language: {
                            // Check Current Package
                            if (mainController.currentPackage) {
                                return mainController.currentPackage.languages[index];
                            }

                            return "";
                        }

                        cellWarning: {
                            // Check Translation Item
                            if (translationItemRoot.translationItem) {
                                // Check Unfit Languages
                                if (translationItemRoot.translationItem.unfitLanguages.indexOf(language) >= 0) {
                                    return true;
                                }

                                if (modelData.length === 0) {
                                    return true;
                                }
                            }

                            return false;
                        }

                        cellCheckActive: {
                            // Check If Checking Active
                            if (mainController.checkingActive && (mainController.checkIndex === translationItemRoot.sourceIndex)) {
                                // Get Current Language
                                var cLanguage = mainController.selectedItemLanguage;

                                return cellDelegateRoot.language === cLanguage;
                            }

                            return false;
                        }

                        visible: {
                            // Check Current Package
                            if (mainController.currentPackage) {
                                // Check Selected Languages
                                if (mainController.currentPackage.selectedLanguages.length === 0) {
                                    return false;
                                }

                                // Check Selected Languages
                                return (mainController.currentPackage.selectedLanguages.indexOf(language) >= 0);
                            }

                            return false;
                        }

                        onCellPressed: translationItemRoot.cellPressed(cell, cellIndex, modifiers);
                        onCellClicked: translationItemRoot.cellClicked(cell, cellIndex, modifiers);
                        onCellDoubleClicked: translationItemRoot.cellDoubleClicked(cell, cellIndex);
                    }
                }
            }
        }
    }

    // Delete Mode Overlay Loader
    Loader {
        id: deleteModeLoader
        anchors.fill: parent
        asynchronous: true
        active: false

        sourceComponent: Rectangle {
            anchors.fill: deleteModeLoader

            opacity: translationItemRoot.editMode ? 1.0 : 0.0
            Behavior on opacity { TEFade { } }
            visible: opacity > 0.0

            color: TEStyle.defaultColorBackground

            MouseArea {
                anchors.fill: parent
                preventStealing: true

                onClicked: {
                    // Emit Edit Mode Finished Signal
                    translationItemRoot.editModeFinished();
                }
            }

            TEButton {
                x: listContainerWidth - width - TEStyle.defaultMargin

                height: parent.height - TEStyle.defaultSpacing

                anchors.verticalCenter: parent.verticalCenter

                defaultColor: "DarkRed"
                defaultColorPressed: "Red"

                text: "Delete"

                onClicked: {
                    // Emit Item Delete Signal
                    translationItemRoot.itemDelete(translationItemRoot.sourceIndex);
                }
            }
        }
    }
}
