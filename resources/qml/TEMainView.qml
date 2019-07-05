import QtQuick 2.0

import customcomponents 1.0

import "qrc:/resources/qml/style"

Item {
    id: mainViewRoot

    anchors.fill: parent

    // Selector Height
    property int selectorHeight: 36

    // Item Per Page
    property int itemPerPage: Math.floor(packageListView.height / TEStyle.listItemHeight)

    // Item Popup Model
    property var popupModel: [
        "Select",
        "-",
        "Details",
        "Check Item",
        "Move",
        "-",
        "Delete"
    ]

    // Return Pressed Flag
    property bool returnPressed: false

    // Item Pressed
    property bool itemPressed: false

    // Horizontal Flick Velocity
    property int horizontalFlickVelocity: 1000

    opacity: mainController.currentPackage ? 1.0 : 0.0
    Behavior on opacity { TEFade { } }
    visible: opacity > 0.0

    focus: true

//    Component.onCompleted: {
//        console.log("TEMainView.onCompleted");
//
//        // ...
//    }

    Keys.onPressed: {
        // Check If Checking Is Active
        if (!mainController.checkingActive) {
            // Check If Repeat
            if (event.isAutoRepeat) {
                // Switch Key
                switch (event.key) {
                    case Qt.Key_Up:         gotoPrevItem(); break;
                    case Qt.Key_Down:       gotoNextItem(); break;

                    case Qt.Key_Left:       scrollLeft(); break;
                    case Qt.Key_Right:      scrollRight(); break;

                    case Qt.Key_PageUp:     goPageUp(); break;
                    case Qt.Key_PageDown:   goPageDown(); break;
                }
            }

            // Switch Key
            switch (event.key) {
                case Qt.Key_Return:
                    // Set Return Pressed
                    mainViewRoot.returnPressed = (mainController.currentItem !== null);
                break;

                case Qt.Key_Control:
                    //console.log("TEMainView.Keys.onPressed - CONTROL");
                break;

                case Qt.Key_Shift:
                    //console.log("TEMainView.Keys.onPressed - SHIFT");
                    // Set Last Shift Pressed Index
                    packageListView.lastShiftPressedIndex = packageListView.currentIndex;
                break;
            }
        }
    }

    // Keys Released
    Keys.onReleased: {
        //console.log("mainViewRoot.Keys.onReleased - key: " + event.key);

        // Check Main Controller Checking Active
        if (mainController.checkingActive) {
            // Check Key
            if (event.key === Qt.Key_Escape) {
                // Stop Checking Process
                mainController.stopCheckingAllTranslations(true);
            }

            return;
        }

        // Switch Key
        switch (event.key) {
            case Qt.Key_Up:         gotoPrevItem(); break;
            case Qt.Key_Down:       gotoNextItem(); break;

            case Qt.Key_Left:       scrollLeft(); break;
            case Qt.Key_Right:      scrollRight(); break;

            case Qt.Key_PageUp:     goPageUp(); break;
            case Qt.Key_PageDown:   goPageDown(); break;

            case Qt.Key_Home:       goHome(event.modifiers); break;
            case Qt.Key_End:        goEnd(event.modifiers); break;

            case Qt.Key_Return:
                // Check If Got The Pressed Event
                if (mainViewRoot.returnPressed) {
                    // Reset Return Pressed
                    mainViewRoot.returnPressed = false;
                    // Check Edit Mode
                    if (!mainController.cellEditMode) {
                        // Restart Filter Timer
                        dblClickFilterTimer.restart();
                    }
                }
            break;

            case Qt.Key_Space:
                // Set Cell Edit Mode
                mainController.cellEditMode = true;
            break;

            case Qt.Key_Escape:
                // Check Cell Edit Mode
                if (mainController.cellEditMode) {
                    // Reset Cell Edit Mode
                    mainController.cellEditMode = false;
                }
            break;

            case Qt.Key_Control:
                //console.log("TEMainView.Keys.onReleased - CONTROL");
            break;

            case Qt.Key_Shift:
                //console.log("TEMainView.Keys.onReleased - SHIFT");

                // Reset Last Shift Pressed Index
                packageListView.lastShiftPressedIndex = -1;
            break;
        }

        // ...
    }

    // Goto Prev Item
    function gotoPrevItem() {
        // Goto Index
        gotoIndex(Math.max(0, packageListView.currentIndex - 1));
    }

    // Goto Next Item
    function gotoNextItem() {
        // Goto Index
        gotoIndex(Math.min(packageListView.count - 1, packageListView.currentIndex + 1));
    }

    // Go Page Up
    function goPageUp() {
        // Get New Index
        var newIndex = Math.max(0, packageListView.currentIndex - mainViewRoot.itemPerPage);
        // Goto Index
        gotoIndex(newIndex);

        // Check Last Shit Pressed Index - Post Processing
        if (packageListView.lastShiftPressedIndex !== -1) {
            // Toggle Item Selected
            toggleItemSelected(newIndex);
        }
    }

    // Go Page Down
    function goPageDown() {
        // Get New Index
        var newIndex = Math.min(packageListView.count - 1, packageListView.currentIndex + mainViewRoot.itemPerPage)
        // Goto Index
        gotoIndex(newIndex);
        // Check Last Shit Pressed Index - Post Processing
        if (packageListView.lastShiftPressedIndex !== -1) {
            // Toggle Item Selected
            toggleItemSelected(newIndex);
        }
    }

    // Go Home
    function goHome(modifiers) {
        console.log("TEMainView.goHome - modifiers: " + modifiers);

        // Check If Control Pressed
        if ((modifiers & Qt.MetaModifier) || (modifiers & Qt.ControlModifier)) {
            // Goto Index
            gotoIndex(0);
            // Check Last Shit Pressed Index - Post Processing
            if (packageListView.lastShiftPressedIndex !== -1) {
                // Toggle Item Selected
                toggleItemSelected(0);
            }
        } else {
            // Set To
            flickableScrollAnim.to = 0;
            // Scroll To Show - Restart Flickable Scroll Animation
            flickableScrollAnim.restart();
        }
    }

    // Go End
    function goEnd(modifiers) {
        console.log("TEMainView.goEnd - modifiers: " + modifiers);

        // Check If Control Pressed
        if ((modifiers & Qt.MetaModifier) || (modifiers & Qt.ControlModifier)) {
            // Goto Index
            gotoIndex(packageListView.count - 1);

            // Check Last Shit Pressed Index - Post Processing
            if (packageListView.lastShiftPressedIndex !== -1) {
                // Toggle Item Selected
                toggleItemSelected(packageListView.count - 1);
            }
        } else {
            // Set To
            flickableScrollAnim.to = horizontalFlickable.contentWidth - horizontalFlickable.width;
            // Scroll To Show - Restart Flickable Scroll Animation
            flickableScrollAnim.restart();
        }
    }

    // Scroll Left
    function scrollLeft() {
        //console.log("TEMainView.scrollLeft");
        // Check Cell Edit Mode & Content Pos
        if (!cellEditor.visible && horizontalFlickable.contentX > -horizontalFlickVelocity) {
            // Flick Left
            horizontalFlickable.flick(horizontalFlickVelocity, 0);
        }
    }

    // Scroll Right
    function scrollRight() {
        //console.log("TEMainView.scrollRight");
        // Check Cell Edit Mode & Content Pos
        if (!cellEditor.visible && horizontalFlickable.contentX < (horizontalFlickable.contentWidth - horizontalFlickable.width + horizontalFlickVelocity)) {
            // Flick Right
            horizontalFlickable.flick(-horizontalFlickVelocity, 0);
        }
    }

    // Goto Index
    function gotoIndex(newIndex) {
        // Check Last Shit Pressed Index
        if (packageListView.lastShiftPressedIndex !== -1) {
            // Check New Index
            if (packageListView.currentIndex < newIndex) {
                // Iterate Through Items
                for (var i=packageListView.currentIndex; i<newIndex; i++) {
                    // Toggle Item Selected
                    toggleItemSelected(i);
                }
            } else if (packageListView.currentIndex > newIndex) {
                // Iterate Through Items
                for (var j=packageListView.currentIndex; j>newIndex; j--) {
                    // Toggle Item Selected
                    toggleItemSelected(j);
                }
            } else if (packageListView.currentIndex === newIndex) {
                // Toggle Item Selected
                toggleItemSelected(newIndex);
            }
        }

        // Set Current Index
        packageListView.currentIndex = newIndex;
    }

    // Set Item Selected
    function setItemSelected(index, selected) {
        // Check Cell Edit Mode
        if (!mainController.cellEditMode) {
            // Get Source Index
            var sIndex = packageListView.filterModel.mapIndexToSource(index);
            // Set Item Selected
            mainController.currentPackage.setItemSelected(sIndex, selected);
        }
    }

    // Toggle Item Selected
    function toggleItemSelected(index) {
        // Check Cell Edit Mode
        if (!mainController.cellEditMode) {
            // Get Source Index
            var sIndex = packageListView.filterModel.mapIndexToSource(index);
            // Get Item
            var item = mainController.currentPackage.getItem(sIndex);
            // Set Item Selected
            mainController.currentPackage.setItemSelected(sIndex, !item.selected);
        }
    }

    // Handle Popup Item Selection
    function handlePopupItemSelection(index) {
        console.log("TEMainView.handlePopupItemSelection - index: " + index);

        // Check Current Package
        if (mainController.currentPackage) {
            // Switch Index
            switch (index) {
                case 0:
                    // Get Item
                    var item = mainController.currentPackage.getItem(mainController.currentPackage.singleSelectionIndex);
                    // Select Item
                    mainController.currentPackage.setItemSelected(mainController.currentPackage.singleSelectionIndex, !item.selected);
                break;

                case 2:
                    // Edit Current Item
                    mainController.editTranslationItem(mainController.currentPackage.singleSelectionIndex);
                break;

                case 3:
                    // Check If Checking Active
                    if (!mainController.checkingActive) {
                        // Check Translation Item
                        mainController.checkTranslationItem();
                    }
                break;

                case 4:
                    // Move Selected Items
                    //mainController.currentPackage.moveItem();
                break;

                case 6:
                    // Get Confirmation
                    if (mainController.getConfirmation("Deleting Items", "Are you Sure?")) {
                        // Remove Selected Item(s)
                        mainController.currentPackage.removeTranslationItem();
                    }
                break;

                default:
                break;
            }
        }
    }

    // Handle Control Click
    function handleControlClick(index) {
        console.log("TEMainView.handleControlClick - index: " + index);
        // Get Source Index
        var sIndex = packageListView.filterModel.mapIndexToSource(index);
        // Get Item
        var item = mainController.currentPackage.getItem(sIndex);
        // Set Item Selected State
        mainController.currentPackage.setItemSelected(sIndex, !item.selected);
    }

    // Handle Shift Click
    function handleShiftClick(index) {
        // Check Last Shift Pressed Index
        if (packageListView.lastShiftPressedIndex !== -1) {
            console.log("TEMainView.handleShiftClick - index: " + index);
            // Check New Index
            if (packageListView.lastShiftPressedIndex < index) {
                // Iterate Through Items
                for (var i=packageListView.lastShiftPressedIndex; i<=index; i++) {
                    // Set Item Selected
                    setItemSelected(i, true);
                }

                // Set Last Shift Pressed Index
                packageListView.lastShiftPressedIndex = index;
                // Set Current Index
                packageListView.currentIndex = index;

            } else if (packageListView.lastShiftPressedIndex > index) {

                // Iterate Through Items
                for (var j=packageListView.lastShiftPressedIndex; j>=index; j--) {
                    // Set Item Selected
                    setItemSelected(j, true);
                }

                // Set Last Shift Pressed Index
                packageListView.lastShiftPressedIndex = index;
                // Set Current Index
                packageListView.currentIndex = index;

            } else if (packageListView.lastShiftPressedIndex === index) {
                // Set Item Selected
                setItemSelected(index, true);
            }
        }
    }

    // Scroll To Column
    function scrollToColumn(index) {
        // Check Visibility
        if (!horizontalFlickable.visible) {
            return;
        }

        // Get Number Of Columns
        var cCount = packageListHeader.children.length;
        // Check Index
        if (index >= 0 && index < cCount) {
            console.log("TEMainView.scrollToColumn - cCount: " + cCount + " - index: " + index);

            // Get Header Item - THERE IS A SPACER
            var hItem = packageListHeader.children[index];

//            console.log("TEMainView.scrollToColumn - hItem.x    : " + hItem.x);
//            console.log("TEMainView.scrollToColumn - hItem.width: " + hItem.width);
//            console.log("TEMainView.scrollToColumn - contentX   : " + horizontalFlickable.contentX);
//            console.log("TEMainView.scrollToColumn - visualEnd  : " + (horizontalFlickable.contentX + horizontalFlickable.width));

            var visualRangeBegin = horizontalFlickable.contentX;
            var visualRangeEnd = visualRangeBegin + horizontalFlickable.width;

//            console.log("TEMainView.scrollToColumn - visualRangeBegin: " + visualRangeBegin);
//            console.log("TEMainView.scrollToColumn - visualRangeEnd  : " + visualRangeEnd);

            var itemBegin = hItem.x - TEStyle.defaultMargin;
            var itemEnd = itemBegin + hItem.width + TEStyle.defaultMargin;

//            console.log("TEMainView.scrollToColumn - itemBegin       : " + itemBegin);
//            console.log("TEMainView.scrollToColumn - itemEnd         : " + itemEnd);

            // Check Index
            if (index === 0) {
                // Set To
                flickableScrollAnim.to = 0;
                // Scroll To Show - Restart Flickable Scroll Animation
                flickableScrollAnim.restart();
            } else {
                // Check Item Begin
                if (itemBegin < visualRangeBegin) {
                    // Set Scroll Anim to - Scroll Left
                    flickableScrollAnim.to = itemBegin;
                    // Scroll To Show - Restart Flickable Scroll Animation
                    flickableScrollAnim.restart();
                // Check Item End
                } else if (itemEnd > visualRangeEnd) {
                    // Set Scroll Anim to - Scroll Right
                    flickableScrollAnim.to = itemEnd - horizontalFlickable.width;
                    // Scroll To Show - Restart Flickable Scroll Animation
                    flickableScrollAnim.restart();
                }
            }
        }
    }

    // Main Controller Connections
    Connections {
        id: mainControllerConnections

        target: mainController

        onCurrentPackageChanged: {
            // Check Current Package
            if (mainController.currentPackage && mainController.currentPackage.rowCount() > 0) {
                // Set List View Current Index
                packageListView.currentIndex = 0;
            } else {
                // Reset List View Current Index
                packageListView.currentIndex = -1;
            }
        }

        onSearchResultItemSelected: {
            //console.log("TEMainView.mainControllerConnections.onSearchResultItemSelected - aIndex: " + aIndex);

            // Set Current Index
            packageListView.currentIndex = packageListView.filterModel.mapIndexFromSource(aIndex);
        }

        onCurrentIndexChanged: {
            //console.log("TEMainView.mainControllerConnections.onCurrentIndexChanged - currentIndex: " + mainController.currentIndex);

            // Check If Checking State Active
            if (!mainController.checkingActive) {
                // Set List View Current Index
                packageListView.currentIndex = packageListView.filterModel.mapIndexFromSource(mainController.currentIndex);
            }
        }

        onSelectedItemLanguageChanged: {
            //console.log("TEMainView.mainControllerConnections.onSelectedItemLanguageChanged - selectedItemLanguage: " + mainController.selectedItemLanguage);

            // Check If Checking State Active
            if (!mainController.checkingActive) {
                // Get Selected Language Index
                var slIndex = mainController.getSelectedLanguageIndex(mainController.selectedItemLanguage);

                //console.log("TEMainView.mainControllerConnections.onSelectedItemLanguageChanged - slIndex: " + slIndex);

                // Show Language Column
                scrollToColumn(slIndex + 6);
            }
        }

        onCheckingStarted: {
            console.log("TEMainView.mainControllerConnections.onCheckingStarted");
            // Reset Current Index
            packageListView.currentIndex = -1;
            // Set Follow Check Index
            packageListView.followCheckingIndex = true;
        }

        onCheckIndexChanged: {
            // Check If Checking State Active
            if (mainController.checkingActive) {
                console.log("TEMainView.mainControllerConnections.onCheckIndexChanged - checkIndex: " + mainController.checkIndex);

                // Check To Follow Checking Index
                if (packageListView.followCheckingIndex) {
                    // Set List View Current Index
                    packageListView.currentIndex = packageListView.filterModel.mapIndexFromSource(mainController.checkIndex);
                }
            }
        }

        onSingleCheckStarted: {
            // Hide Cell Editor
            cellEditor.hide();

            // ...
        }

//        onCheckingFinished: {
//            // Check If Single Check
//            if (!aSingleCheck) {
//                // Reset Current Index
//                packageListView.currentIndex = 0;
//            }
//        }

        onCellEditModeChanged: {
            // Check Cell Edit Mode
            if (!mainController.cellEditMode) {
                // Hide Cell Editor
                cellEditor.hide();
            }
        }

        // Reset Sorting Signal
        onResetSorting: {
            console.log("TEMainView.mainControllerConnections.onResetSorting");
            // Set Sorting Column
            packageListView.filterModel.sortingColumn = 0;
            // Reset Descending Order
            packageListView.filterModel.descending = false;

            // Reset State Filters
            packageListView.filterModel.showChecked = true;
            packageListView.filterModel.showDraft = true;
            packageListView.filterModel.showWarning = true;

            // Reset Filter Text
            filterEditor.filterText = "";
            // Hide Filter Editor
            filterEditor.hide();
        }
    }

    // Group Selector
    Flow {
        id: groupSelectorFlow

        anchors.left: parent.left
        anchors.leftMargin: TEStyle.defaultMargin
        anchors.right: languagesSelectorFlow.left
        anchors.rightMargin: TEStyle.defaultMargin
        anchors.top: parent.top
        anchors.topMargin: TEStyle.defaultMargin

        property bool editMode: false

        spacing: TEStyle.defaultSpacing

        enabled: !mainController.checkingActive
        opacity: enabled ? 1.0 : TEStyle.disabledOpacity
        Behavior on opacity { TEFade { } }

        move: Transition {
            ParallelAnimation {
                PropertyAnimation {
                    properties: "x, y"
                    duration: TEStyle.defaultDurationShort
                }
            }
        }

        // Groups Title Label
        TELabel {
            id: groupsTitleLabel
            height: mainViewRoot.selectorHeight
            text: "Groups: "

            scale: toggleAllGroupsButton.pressed ? 0.9 : 1.0
            color: toggleAllGroupsButton.pressed ? TEStyle.defaultColorSelect : TEStyle.defaultColorText

            MouseArea {
                id: toggleAllGroupsButton
                anchors.fill: parent
                onClicked: {
                    // Check Current Package
                    if (mainController.currentPackage) {
                        // Check Selected Groups
                        if (mainController.currentPackage.selectedGroups.length > 0) {
                            // Deselect All Groups
                            mainController.currentPackage.deselectAllGroups();
                        } else {
                            // Select All Groups
                            mainController.currentPackage.selectAllGroups();
                        }
                    }
                }
            }
        }

        // No groups Label
        TELabel {
            id: noGroupsLabel
            height: mainViewRoot.selectorHeight
            visible: groupsRepeater.count === 0
            text: "No Groups"
        }

        // Groups Repeater
        Repeater {
            id: groupsRepeater

            model: mainController.currentPackage ? mainController.currentPackage.groups : 0

            onCountChanged: {
                // Check Count
                if (count <= 0) {
                    // Reset Edit Mode
                    groupSelectorFlow.editMode = false;
                }
            }

            onItemAdded: {
                // Check Current Package
                if (mainController.currentPackage) {
                    // Set Selected Group
                    mainController.currentPackage.setSelectedGroup(item.groupName, true);
                }
            }

            delegate: Item {
                width: TEStyle.buttonWidth
                height: TEStyle.buttonHeight

                property string groupName: modelData

                // Group Button
                TEButton {
                    text: modelData

                    checked: {
                        // Check Current Package
                        if (mainController.currentPackage) {
                            return mainController.currentPackage.selectedGroups.indexOf(modelData) !== -1;
                        }

                        return false;
                    }

                    Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                        border.color: TEStyle.defaultColorSelect
                        visible: parent.checked
                        radius: TEStyle.defaultRadius
                    }

                    onCheckedChanged: {
                        // Check Checked State
                        if (checked) {
                            // Add Group Filter
                            packageListView.filterModel.addGroupFilter(parent.groupName);
                        } else {
                            // Remove Group Filter
                            packageListView.filterModel.removeGroupFilter(parent.groupName);
                        }
                    }

                    onClicked: {
                        // Check Edit Mode
                        if (groupSelectorFlow.editMode) {
                            // Reset Edit Mode
                            groupSelectorFlow.editMode = false;
                        } else {
                            // Check Current Package
                            if (mainController.currentPackage) {
                                // Set Selected Group
                                mainController.currentPackage.setSelectedGroup(parent.groupName, !checked);
                            }
                        }
                    }

                    onPressAndHold: {
                        // Engage Edit Mode
                        groupSelectorFlow.editMode = true;
                    }
                }

                // Remove Group Button
                TEImageButton {
                    id: removeGroupButton
                    width: 24
                    height: 24

                    anchors.right: parent.right
                    anchors.rightMargin: -(width * 0.25)
                    anchors.top: parent.top
                    anchors.topMargin: -(height * 0.25)

                    visible: groupSelectorFlow.editMode

                    iconSouorce: "qrc:/resources/images/icons/icon_close.png"

                    onClicked: {
                        if (mainController.currentPackage) {

                            // TODO: Confirmation!!

                            // Remove Group
                            mainController.currentPackage.removeGroup(modelData);
                        }
                    }
                }
            }
        }

        // Add Group Button
        Item {
            id: addGroupContainer
            width: addGroupButton.width + TEStyle.defaultMargin * 2
            height: mainViewRoot.selectorHeight

            TEButton {
                id: addGroupButton

                width: 128

                anchors.right: parent.right

                text: "Add Group"

                onClicked: {
                    // Add Translation Group
                    mainController.addTranslationGroup();
                }
            }
        }
    }

    // Language Selector Row
    Row {
        id: languagesSelectorFlow

        width: mainViewRoot.width * 0.48

        anchors.right: editModeToggleButton.left
        anchors.rightMargin: TEStyle.defaultMargin * 2
        anchors.top: parent.top
        anchors.topMargin: TEStyle.defaultMargin

        //layoutDirection: Qt.LeftToRight
        spacing: TEStyle.defaultSpacing

        enabled: !mainController.checkingActive
        opacity: enabled ? 1.0 : TEStyle.disabledOpacity
        Behavior on opacity { TEFade { } }

        TELabel {
            id: languagesTitleLabel
            height: 32
            text: "Languages: "

            scale: toggleAllLanguagesButton.pressed ? 0.9 : 1.0
            color: toggleAllLanguagesButton.pressed ? TEStyle.defaultColorSelect : TEStyle.defaultColorText

            MouseArea {
                id: toggleAllLanguagesButton
                anchors.fill: parent
                onClicked: {
                    // Check Current Package
                    if (mainController.currentPackage) {
                        // Toggle All/None
                        mainController.currentPackage.setAllLanguagesSelected(!mainController.currentPackage.allLanguages);
                    }
                }
            }
        }

        TELabel {
            id: noLanguagesLabel
            visible: languagesRepeater.count === 0
            text: "No Languages"
        }

        // Flags Flow
        Flow {
            id: languageFlagsFlow
            width: languagesSelectorFlow.width - languagesTitleLabel.width - TEStyle.defaultSpacing * 2

            spacing: TEStyle.defaultSpacing

            move: Transition {
                ParallelAnimation {
                    PropertyAnimation {
                        properties: "x, y"
                        duration: TEStyle.defaultDurationShort
                    }
                }
            }

            Repeater {
                id: languagesRepeater

                model: mainController.currentPackage ? mainController.currentPackage.languages : 0

                onItemAdded: {
                    // Check Current Package
                    if (mainController.currentPackage) {
                        // Set Selected Language
                        mainController.currentPackage.setSelectedLanguage(item.language, true);
                    }
                }

                delegate: TEButton {
                    width: 42
                    height: 32

                    text: ""

                    property string language: modelData

                    Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                        border.color: TEStyle.defaultColorSelect
                        visible: checked
                        radius: TEStyle.defaultRadius
                    }

                    TEImage {
                        width: 32
                        height: 32
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        source: mainController.getLanguageFlag(modelData)
                    }

                    checked: {
                        // Check Current Package
                        if (mainController.currentPackage) {
                            return mainController.currentPackage.selectedLanguages.indexOf(modelData) !== -1;
                        }

                        return false;
                    }

                    onClicked: {
                        // Check Current Package
                        if (mainController.currentPackage) {
                            // Set Selected Language
                            mainController.currentPackage.setSelectedLanguage(modelData, !checked);
                        }
                    }
                }
            }
        }
    }

    // Edit Mode Toggle Button
    TEButton {
        id: editModeToggleButton

        width: TEStyle.buttonWidth
        height: TEStyle.buttonWidth

        anchors.right: parent.right
        anchors.rightMargin: TEStyle.defaultMargin
        anchors.top: parent.top
        anchors.topMargin: TEStyle.defaultMargin

        enabled: !mainController.checkingActive

        text: ""

        checked: mainController.cellEditMode

        onClicked: {
            // Toggle Edit Mode
            mainController.cellEditMode = !mainController.cellEditMode;

            // ...
        }

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: TEStyle.defaultColorSelect
            visible: parent.checked
            radius: TEStyle.defaultRadius
        }

        TEImage {
            anchors.centerIn: parent
            scale: parent.pressed ? 0.9 : 1.0
            source: preferences.guiTheme === 0 ? "qrc:/resources/images/icons/icon_edit_black-48.png"
                                               : "qrc:/resources/images/icons/icon_edit_white-48.png"
        }
    }

    // No Items Label
    TELabel {
        id: noItemsLabel
        anchors.centerIn: parent
        font.pixelSize: 24
        opacity: packageListView.count > 0 ? 0.0 : 1.0
        Behavior on opacity { TEFade { } }
        text: "No Translation Items"
    }

    // Disengage Edit Mode Mouse Area
    MouseArea {
        anchors.fill: parent

        visible: packageListView.editMode
        preventStealing: true

        onClicked: {
            // Reset Edit Mode
            packageListView.editMode = false;
        }
    }

    // Main Horizontal Flickable
    Flickable {
        id: horizontalFlickable

        anchors.left: parent.left
        anchors.leftMargin: TEStyle.defaultMargin
        anchors.right: parent.right
        anchors.rightMargin: TEStyle.defaultMargin
        anchors.top: parent.top
        anchors.topMargin: Math.max(editModeToggleButton.height, Math.max(groupSelectorFlow.height, languagesSelectorFlow.height)) + TEStyle.defaultMargin * 2
        anchors.bottom: filterEditor.top
        anchors.bottomMargin: TEStyle.defaultMargin

        flickableDirection: Flickable.HorizontalFlick

        contentWidth: packageListView.width

        interactive: !packageListView.editMode //&& !mainController.cellEditMode

        // Header Row
        Row {
            id: packageListHeader

            height: TEStyle.defaultHeaderHeight

            spacing: TEStyle.defaultSpacing

            Rectangle {
                width: TEStyle.defaultMargin - TEStyle.defaultSpacing
                height: parent.height
                color: TEStyle.defaultColorHeader
            }

            // Header Items Repeater
            Repeater {
                id: packageListHeaderRepeater

                model: {
                    // Init Number Of Columns
                    var numColumns = 5;

                    // Check Current Package
                    if (mainController.currentPackage) {
                        // Add Languages/Translations
                        numColumns += mainController.currentPackage.languages.length;
                    }

                    return numColumns;
                }

                delegate: TEHeaderItem {
                    width: preferences.getColumnWidth(index) //+ preferences.columnWidthChanged

                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    headerItemIndex: index

                    sortingColumn: packageListView.filterModel.sortingColumn
                    sortDescending: packageListView.filterModel.descending

                    minWidth: {
                        switch (index) {
                            case 3: return 96;
                        }

                        return 56;
                    }

                    textAlignment: {
                        if (index === 0 || index === 2 || index === 3) {
                            return Text.AlignHCenter;
                        }

                        return Text.AlignLeft;
                    }

                    visible: {
                        // Check Index
                        if (index < 5) {
                            return true;
                        }

                        // Check Current Package
                        if (!mainController.currentPackage || language === "") {
                            return false;
                        }

                        return  (mainController.currentPackage.selectedLanguages.indexOf(language) >= 0);
                    }

                    language: {
                        // Check Index
                        if (index < 5) {
                            return "";
                        }

                        // Check Current Package
                        if (mainController.currentPackage) {
//                            console.log("languages: " + mainController.currentPackage.languages);
//                            console.log("index: " + index);
                            return mainController.currentPackage.languages[index - 5];
                        }

                        return "";
                    }

                    title: {
                        if (index >= 5 && language === "") {
                            return "";
                        }

                        switch (index) {
                            case 0: return "Index";
                            case 1: return "Text ID";
                            case 2: return "Screen";
                            case 3: return "State";
                            case 4: return "Original Text";
                        }

                        return mainController.getLanguageName(mainController.currentPackage.languages[index - 5]);
                    }

                    onClicked: {
                        // Check Button - Left
                        if (mouse.button === Qt.LeftButton) {
                            // Check Index
                            if (index >= 0 && index < 5) {
                                // Check Sort Mode
                                if (packageListView.filterModel.sortingColumn === index) {
                                    // Toggle Descending Order
                                    packageListView.filterModel.descending = !packageListView.filterModel.descending;

                                } else {
                                    // Set Sorting Column
                                    packageListView.filterModel.sortingColumn = index;
                                }
                            }
                        } else if (mouse.button === Qt.RightButton) {
                            //Hide Item Popup
                            itemPopup.dismiss();

                            // Mapped Pos
                            var mappedPos = mapToItem(null, mouse.x, mouse.y);

                            // Launch Header Popup
                            headerPopup.show(mappedPos.x, mappedPos.y);
                        }
                    }
                }
            }
        }

        // Header Fill Rectangle
        Rectangle {
            height: packageListHeader.height
            anchors.left: packageListHeader.right
            anchors.leftMargin: TEStyle.defaultSpacing
            anchors.right: parent.right
            color: TEStyle.defaultColorHeader
        }

        // Package List View
        ListView {
            id: packageListView

            width: Math.max(packageListHeader.width, horizontalFlickable.width)
            height: horizontalFlickable.height - packageListHeader.height

            anchors.top: packageListHeader.bottom

            property bool editMode: false

            property PackageGroupFilter filterModel: PackageGroupFilter {
                sourceModel: mainController.currentPackage
                searchFilter: filterEditor.filterText
            }

            // Follow Checking Index
            property bool followCheckingIndex: false

            // Last Index
            property int lastIndex: -1
            // Last Index When Shift Key Pressed
            property int lastShiftPressedIndex: -1

//            // Property Translation Cells Loading Index
//            property int translationCellsLoadingIndex: -1
//            // Delegate Created Index
//            property int delegateCreatedIndex: -1
//            // Last Delegate Index
//            property int lastDelegateCreatedIndex: -1

            // Delegate Instanecs
            property var delegateInstances: []
            // Delegates Count
            property int delegatesCount: 0
            // Delegate Update Index
            property int delegateUpdateIndex: -1

            highlightMoveDuration: mainController.checkingActive ? 0 : TEStyle.defaultDurationShort
            highlightResizeDuration: 0

            highlightRangeMode: mainController.checkingActive ? ListView.ApplyRange : ListView.NoHighlightRange

            preferredHighlightBegin: TEStyle.listItemHeight
            preferredHighlightEnd: height - TEStyle.listItemHeight * 2

            snapMode: ListView.NoSnap //ListView.SnapToItem  // ListView.SnapOneItem

            //interactive: !mainController.cellEditMode

            opacity: packageListView.count > 0 ? 1.0 : 0.0
            Behavior on opacity { TEFade { } }
            visible: opacity > 0.0

            cacheBuffer: Math.max(TEStyle.listItemHeight, packageListView.height) //TEStyle.listItemHeight * 16

            clip: true

//            add: Transition { TEFade { } }
//            addDisplaced: Transition { TEFade { property: "y" } }

//            move: Transition { TEFade { property: "y" } }
//            moveDisplaced: Transition { TEFade { property: "y" } }

//            remove: Transition { TEFade { } }
//            removeDisplaced: Transition { TEFade { property: "y" } }

//            populate: Transition { TEFade { } }

            model: filterModel

            // Delegate
            delegate: TETranslationItem {
                id: translationItemDelegate

                width: packageListView.width

                property bool longPress: false
                property bool doubleClick: false

                sourceIndex: model.triSourceIndex

                translationItem: mainController.currentPackage ? mainController.currentPackage.getItem(sourceIndex) : null

                textLabelID: model.triTextID
                textOriginal: model.triOriginal
                textScreen: model.triScreen
                textState: model.triState
                textTranslations: model.triMerged

                itemWarning: model.triState === "Warning"

                alternateBG: ((index % 2) > 0) && preferences.alternateRowColors

                moving: packageListView.movingVertically

//                isCurrent: ListView.isCurrentItem//index === packageListView.currentIndex
                selected: model.triSelected

                editMode: packageListView.editMode

                listContainerWidth: horizontalFlickable.width

                onDelegateCreated: {
                    // Push Delegate
                    packageListView.delegateInstances.push(itemInstance);
                    // Update Delegates Count
                    packageListView.delegatesCount = packageListView.delegateInstances.length;

                    // ...
                }

                onDelegateDestory: {
                    // Get Index Of Delegate Reference
                    var dIndex = packageListView.delegateInstances.indexOf(itemInstance);
                    // Check Delegate Index
                    if (dIndex >= 0) {
                        // Remove Item
                        packageListView.delegateInstances.splice(dIndex, 1);
                        // Update Delegates Count
                        packageListView.delegatesCount = packageListView.delegateInstances.length;
                    }

                    // ...
                }

                onPressed: {
                    // Hide Popup
                    itemPopup.hide();

                    //console.log("TEMainView.translationItemDelegate.onPressed - index: " + index);

                    // Set Item Pressed
                    mainViewRoot.itemPressed = true;

                    // Check IF Checking Active
                    if (mainController.checkingActive) {
                        return;
                    }

                    // Check Button
                    if (mouse.button === Qt.RightButton) {
                        // Set Current Index
                        packageListView.currentIndex = index;
                    }
                }

                onReleased: {
                    //console.log("TEMainView.translationItemDelegate.onReleased - index: " + index);

                    // Reset Item Pressed
                    mainViewRoot.itemPressed = false;

                    // Reset Long Press
                    longPress = false;
                }

                onClicked: {
                    // Check IF Checking Active
                    if (mainController.checkingActive) {
                        return;
                    }

                    // Check Button - Left
                    if (mouse.button === Qt.LeftButton) {
                        // Check Modifiers
                        if (mouse.modifiers & Qt.ShiftModifier) {

                            // Toggle Selection
                            mainController.currentPackage.setItemSelected(packageListView.filterModel.mapIndexToSource(index), !model.triSelected);

                        } else {
                            // Check Long Press
                            if (!longPress) {
                                console.log("TEMainView.translationItemDelegate.onClicked - index: " + index);

                                // Check Cell Edit Mode
                                if (!mainController.cellEditMode) {
                                    // Request Focus
                                    mainViewRoot.focus = true;
                                }

                                // Set Current Index
                                packageListView.currentIndex = index;
                            }
                        }
                    // Check Button - Right
                    } else if (mouse.button === Qt.RightButton) {
                        //Dismiss Header Popup
                        headerPopup.dismiss();

                        // Mapped Pos
                        var mappedPos = mapToItem(null, mouse.x, mouse.y);

                        // Set Single Selection Index
                        mainController.currentPackage.singleSelectionIndex = packageListView.filterModel.mapIndexToSource(index);

                        // Get Item
                        var item = mainController.currentPackage.getItem(mainController.currentPackage.singleSelectionIndex);

                        // Set Popup Item Text
                        itemPopup.setItemText(0, item.selected ? "Deselect" : "Select");
                        itemPopup.setItemText(3, mainController.currentPackage.selectedItemCount > 0 ? "Check Items" : "Check Item");

                        // Launch Item Popup
                        itemPopup.show(mappedPos.x, mappedPos.y);

                        // ...
                    }

                    // Reset Item Pressed
                    mainViewRoot.itemPressed = false;
                }

                onPressAndHold: {
                    // Check If Checking Active
                    if (mainController.checkingActive) {
                        return;
                    }

                    //console.log("translationItemDelegate.onPressAndHold");

                    // Check Button - Left
                    if (mouse.button === Qt.LeftButton) {
                        // Check Double Click
                        if (doubleClick) {
                            // Reset Double Click
                            doubleClick = false;

                            // Reset Interactive
                            packageListView.interactive = false;
                            // Set Interactive Mode
                            packageListView.interactive = Qt.binding(function() { return !mainController.cellEditMode; });

                        } else {
                            // console.log("translationItemDelegate.onPressAndHold");
                            // Set Long Press
                            longPress = true;
                            // Set Edit Mode
                            packageListView.editMode = true;
                        }
                    }
                }

                onDoubleClicked: {
                    // Check If Checking Or Cell Edit Mode Active
                    if (mainController.checkingActive || mainController.cellEditMode) {
                        return;
                    }

                    //console.log("translationItemDelegate.onDoubleClicked - index: " + index);

                    // Set Double Click
                    doubleClick = true;
                    // Restart Double Click Filter Timer
                    dblClickFilterTimer.restart();
                    // Edit Translation Item
                    // mainController.currentPackage.invokeEditItem(index);
                }

                onEditModeFinished: {
                    console.log("translationItemDelegate.onEditModeFinished");
                    // Reset Edit Mode
                    packageListView.editMode = false;
                }

                onItemDelete: {
                    // Check Current Package
                    if (mainController.currentPackage) {
                        console.log("translationItemDelegate.onItemDelete - deleteIndex: " + deleteIndex);

                        // Get Confirmation
                        if (mainController.getConfirmation("Item Deletion", "Are you sure?")) {
                            // Remove Item
                            mainController.currentPackage.removeTranslationItem(deleteIndex);
                        }
                    }
                }

                onCellPressed: {
                    //console.log("TEMainView.translationItemDelegate.onCellPressed - index: " + index);

                    // Set Item Pressed
                    mainViewRoot.itemPressed = true;
                }

                onCellClicked: {
                    console.log("TEMainView.translationItemDelegate.onCellClicked - index: " + index + " - cellIndex: " + cellIndex + " - modifiers: " + modifiers);

                    // Check Cell Edit Mode
                    if (mainController.cellEditMode) {
                        // Set Current Index
                        packageListView.currentIndex = index;
                        // Setup Cell And Show
                        cellEditor.setupCellEditorAndShow(cellIndex);

                    // Check If Translation(s) Layout Checking Active
                    } else if (!mainController.checkingActive) {

                        // Check Modifiers
                        if (modifiers !== 0) {
                            // Check Modifiers
                            if (modifiers & Qt.ShiftModifier) {
                                // Handle Shift Click
                                handleShiftClick(index);

                            } else if (modifiers & Qt.ControlModifier) {
                                // Handle Control Click
                                handleControlClick(index);
                            }

                        } else {
                            // Set Current Index
                            packageListView.currentIndex = index;

                            // Check Cell/Column Index
                            if (cellIndex >= 3 && (cellIndex - 3) < mainController.currentPackage.languages.length) {
                                // Set Selected/Preview Language
                                mainController.setSelectedTranslationLanguage(cellIndex - 3, true);

                                // ...

                            }
                        }
                    }

                    // Reset Item Pressed
                    mainViewRoot.itemPressed = false;
                }

                onCellDoubleClicked: {
                    // Check If Checking Or Cell Edit Mode Active
                    if (mainController.checkingActive || mainController.cellEditMode) {
                        return;
                    }

                    //console.log("TEMainView.translationItemDelegate.onCellDoubleClicked - cellIndex: " + cellIndex);

                    // Set Double Click
                    doubleClick = true;
                    // Restart Double Click Filter Timer
                    dblClickFilterTimer.restart();
                }
            }

            // Highlight
            highlight: Rectangle {
                color: {
                    // Check Focus
                    if (mainViewRoot.focus) {
                        return TEStyle.defaultColorListHighlight;
                    }

                    return "Transparent";
                }

                border.color: TEStyle.defaultColorListHighlight
            }

            section.property: filterEditor.filterText.length > 0 ? "" : "triGroup"
            //section.labelPositioning: ViewSection.CurrentLabelAtStart

            section.delegate: Rectangle {
                width: packageListView.width
                height: TEStyle.listItemHeight

                color: TEStyle.defaultColorSection

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // Reset Edit Mode
                        packageListView.editMode = false;
                    }
                }

                TELabel {
                    anchors.left: parent.left
                    anchors.leftMargin: TEStyle.defaultMargin
                    anchors.verticalCenter: parent.verticalCenter
                    text: section
                }
            }

//            Component.onCompleted: {
//                console.log("#### TEMainView.packageListView.onCompleted");
//
//            }

            onDelegatesCountChanged: {
//                console.log("TEMainView.packageListView.onDelegatesCountChanged - delegatesCount: " + delegatesCount);

                // Reset Delegate Update Index
                packageListView.delegateUpdateIndex = -1;
                // Restart Cells Load Delay Timer
                translationCellsLoadDelayTimer.restart();

//                // Iterate Through Delegates
//                for (var i=0; i<packageListView.delegatesCount; i++) {
//                    // Get Delegate
//                    var dInstance = packageListView.delegateInstances[i];

//                }

                // ...
            }
/*
            onDelegateCreatedIndexChanged: {
                //console.log("TEMainView.packageListView.onDelegateCreatedIndexChanged - delegateCreatedIndex: " + delegateCreatedIndex);

//                // Launch Delegate Translation Cells Loading
//                launchTranslationCellsLoading((packageListView.lastDelegateCreatedIndex === -1) ||
//                                              (packageListView.lastDelegateCreatedIndex < packageListView.delegateCreatedIndex));

//                // Check Last Delegate Created Index
//                if ((packageListView.lastDelegateCreatedIndex === -1) || (packageListView.lastDelegateCreatedIndex < packageListView.delegateCreatedIndex)) {

//                    // Forward

//                    // Get First Visible Item


//                    // ...

//                } else {

//                    // Reverse

//                    // ...

//                }

                // ...

                // Set Last Delegate Created Index
                packageListView.lastDelegateCreatedIndex = packageListView.delegateCreatedIndex;
            }
*/
            onMovementStarted: {
                console.log("TEMainView.packageListView.onMovementStarted - itemPressed: " + mainViewRoot.itemPressed);

                // Check Item Pressed
                if (mainViewRoot.itemPressed) {
                    // Reset Follow Check Index
                    packageListView.followCheckingIndex = false;
                }

                // ...
            }

            onMovementEnded: {
                console.log("TEMainView.packageListView.onMovementEnded - itemPressed: " + mainViewRoot.itemPressed);

                // ...
            }

//            onContentYChanged: {
//                // Check Item Pressed
//                if (mainViewRoot.itemPressed) {
//                    // Reset Follow Check Index
//                    packageListView.followCheckingIndex = false;
//                }
//            }

            // On Count Changed Slot
            onCountChanged: {
                // Check Count
                if (packageListView.count > 0) {
                    // Check Current Index
                    if (packageListView.currentIndex === -1) {
                        // Set Current Index
                        packageListView.currentIndex = 0;
                    }
                } else {
                    // Reset Current Index
                    packageListView.currentIndex = -1;
//                    // Reset Delegate Created Index
//                    packageListView.delegateCreatedIndex = -1
//                    // Reset Last Delegate Created Index
//                    packageListView.lastDelegateCreatedIndex = -1;
                }
            }

//            onContentHeightChanged: {
//                console.log("#### contentHeight: " + packageListView.contentHeight);
//            }

            // On Current Index Changed Slot
            onCurrentIndexChanged: {
                // Check Current Package
                if (mainController.currentPackage) {
                    // Check If Checking Active
                    if (!mainController.checkingActive) {
                        // Select Translation Item
                        mainController.selectTranslationItem(packageListView.filterModel.mapIndexToSource(packageListView.currentIndex));
                    }

                    // ...
                }
            }

            // Update Delegate - Load Translation Cells
            function updateDelegate() {
//                console.log("TEMainView.packageListView.updateDelegate - delegateUpdateIndex: " + packageListView.delegateUpdateIndex);

                // Check Delegate Update Index
                if (packageListView.delegateUpdateIndex >= 0 && packageListView.delegateUpdateIndex < packageListView.delegateInstances.length) {
                    // Get Delegate Instance
                    var dInstance = packageListView.delegateInstances[packageListView.delegateUpdateIndex];

//                    console.log("TEMainView.packageListView.updateDelegate - dInstance.translationCellsLoaded: " + dInstance.translationCellsLoaded);

                    // Iterate Through Delegates While Translatioon Cells Loaded
                    while (dInstance.translationCellsLoaded && packageListView.delegateUpdateIndex < packageListView.delegateInstances.length) {
                        // Inc Delegate Update Index
                        packageListView.delegateUpdateIndex++;

//                        console.log("TEMainView.packageListView.updateDelegate - delegateUpdateIndex: " + packageListView.delegateUpdateIndex);

                        // Check Delegate Update Index
                        if (packageListView.delegateUpdateIndex >= 0 && packageListView.delegateUpdateIndex < packageListView.delegateInstances.length) {
                            // Get Delegate Instance
                            dInstance = packageListView.delegateInstances[packageListView.delegateUpdateIndex];
                        } else {
                            // Reset Delegate Instance
                            dInstance = null;
                        }
                    }

                    // Check Delegate Update Index
                    if (dInstance) {
                        // Load Translation Cells
                        dInstance.loadTranslationCells();
                        // Restart Cells Load Delay Timer
                        translationCellsLoadDelayTimer.restart();
                    } else {
                        // Reset Delegate Update Index
                        packageListView.delegateUpdateIndex = -1;
                    }
                }
            }
/*
            // Launch Delegate Translation Cells Loading
            function launchTranslationCellsLoading(forward) {
                //console.log("TEMainView.packageListView.launchTranslationCellsLoading - delegateCreatedIndex: " + packageListView.delegateCreatedIndex + " - forward: " + forward);

//                // Init Start Pos
//                var startPos = forward ? TEStyle.listItemHeight * 0.5 : (packageListView.height - TEStyle.listItemHeight * 0.5);
//                // Init Step
//                var step = forward ? TEStyle.listItemHeight : -TEStyle.listItemHeight ;
//                // Init End Pos
//                var endPos = forward ? (packageListView.height + TEStyle.listItemHeight * 0.5) : -TEStyle.listItemHeight * 0.5;

//                // Iterate Through Items
//                for (var i=startPos; (forward ? i<endPos : i>endPos); i+=step) {
//                    //console.log("#### i: " + i);

//                    // Get Item
//                    var item = packageListView.itemAt(TEStyle.listItemHeight, i + packageListView.contentY);
//                    // Check Item
//                    if (item) {
//                        console.log("#### sourceIndex: " + item.sourceIndex + " - translationCellsLoaded: " + item.translationCellsLoaded);

//                        // ...

//                    } else {
//                        console.warn("#### no item!");
//                    }
//                }

            }
*/
            // Double Click Filter Timer
            Timer {
                id: dblClickFilterTimer
                interval: TEStyle.defaultDurationNormal
                repeat: false
                onTriggered: {
                    // Edit Translation Item
                    mainController.editTranslationItem(packageListView.filterModel.mapIndexToSource(packageListView.currentIndex));
                }
            }

            // Translation Cells Load Delay Timer
            Timer {
                id: translationCellsLoadDelayTimer
                interval: 1
                repeat: false

                onTriggered: {
                    // Check Delegate Count
                    if (packageListView.delegatesCount > 0) {
                        // In Delegate Update Index
                        packageListView.delegateUpdateIndex++;
                        // Update Delegate
                        packageListView.updateDelegate();
                    }
                }
            }

            // Number Animation To Scroll Horizontal Flickable
            NumberAnimation {
                id: flickableScrollAnim
                duration: TEStyle.defaultDurationNormal
                target: horizontalFlickable
                property: "contentX"
                easing.type: Easing.OutCubic
                //alwaysRunToEnd: true
            }

            // Cell Editor
            TECellEditor {
                id: cellEditor

                onEditorHidden: {
                    // Reset Focus
                    mainViewRoot.focus = true;
                }

                onAccepted: {
                    //console.log("TEMainView.cellEditor.onAccepted");
                    // Save Text
                    saveText();
                    // Go To Next Row
                    gotoNextRow();
                }

                onCanceled: {

                }

                onCleared: {
                    // Save Text
                    saveText();

                }

                onUpClicked: {
                    // Go To Prev Row
                    gotoPrevRow();
                }

                onDownClicked: {
                    // Go To Next Row
                    gotoNextRow();
                }

                onNextClicked: {
                    // Go To Next Column
                    gotoNextColumn();
                }

                onPrevClicked: {
                    // Go To Prev Column
                    gotoPrevColumn();
                }

                // Save Text
                function saveText() {
                    // Check Current Item
                    if (mainController.currentItem) {
                        console.log("TEMainView.cellEditor.saveText - text: " + cellEditor.text);
                        // Set Text Data
                        mainController.currentItem.setTextData(cellEditor.lastCellIndex, cellEditor.text);
                    }
                }

                // Go To The Next Row
                function gotoNextRow() {
                    console.log("TEMainView.cellEditor.gotoNextRow");

                    // Check Current Index
                    if (packageListView.currentIndex < packageListView.count - 1) {

                        // Inc Current Index
                        packageListView.currentIndex++;

                        // Setup Cell Editor And Show
                        setupCellEditorAndShow(cellEditor.lastCellIndex);

                        // ...
                    }
                }

                // Go To The Previous Row
                function gotoPrevRow() {
                    console.log("TEMainView.cellEditor.gotoPrevRow");

                    // Check Current Index
                    if (packageListView.currentIndex > 0) {

                        // Dec Current Index
                        packageListView.currentIndex--;

                        // Setup Cell Editor And Show
                        setupCellEditorAndShow(cellEditor.lastCellIndex);

                        // ...
                    }
                }

                // Go To Next Column
                function gotoNextColumn() {
                    // Check Current Item
                    if (packageListView.currentItem) {
                        console.log("TEMainView.cellEditor.gotoNextColumn");

                        // Get Next Column Index
                        var mextColumnIndex = packageListView.currentItem.getNextCellIndex(cellEditor.lastCellIndex);

                        // Setup Cell Editor And Show
                        setupCellEditorAndShow(mextColumnIndex);

                        // Ensure Cell Visible
                        ensureCellVisible();

                        // ...
                    }
                }

                // Go To Prev Column
                function gotoPrevColumn() {
                    // Check Current Item
                    if (packageListView.currentItem) {
                        console.log("TEMainView.cellEditor.gotoPrevColumn");

                        // Get Prev Column Index
                        var prevColumnIndex = packageListView.currentItem.getPrevCellIndex(cellEditor.lastCellIndex);

                        // Setup Cell Editor And Show
                        setupCellEditorAndShow(prevColumnIndex);

                        // Ensure Cell Visible
                        ensureCellVisible();

                        // ...
                    }
                }

                // Ensure Cell Visible
                function ensureCellVisible() {
                    console.log("TEMainView.cellEditor.ensureCellVisible");

                    // Check Last Cell Index
                    if (cellEditor.lastCellIndex === 0) {
                        // Set To
                        flickableScrollAnim.to = 0;
                    } else {
                        // Get Cell
                        var cell = packageListView.currentItem.getCell(cellEditor.lastCellIndex);
                        var cellPosX = packageListView.currentItem.getCellPosX(cellEditor.lastCellIndex);

//                        console.log("TEMainView.cellEditor.ensureCellVisible - cell.x    : " + cell.x);
//                        console.log("TEMainView.cellEditor.ensureCellVisible - cell.width: " + cell.width);
//                        console.log("TEMainView.cellEditor.ensureCellVisible - contentX  : " + horizontalFlickable.contentX);

                        // Check Cell Position
                        if (horizontalFlickable.contentX > cellPosX - TEStyle.defaultMargin) {

                            // Set To to Scroll Left
                            flickableScrollAnim.to = cellPosX - TEStyle.defaultMargin;

                        } else if (cellPosX + cell.width + TEStyle.defaultMargin > horizontalFlickable.width + horizontalFlickable.contentX) {

                            // Set To to Scroll Right
                            flickableScrollAnim.to = cellPosX + cell.width + TEStyle.defaultMargin - horizontalFlickable.width;
                        } else {

                            // ...

                        }

                        // ...
                    }

                    // Restart Flickable Scroll Anim
                    flickableScrollAnim.restart();
                }

                // Setup Cell And Show
                function setupCellEditorAndShow(cellIndex) {
                    console.log("TEMainView.cellEditor.setupCellEditorAndShow - cellIndex: " + cellIndex);

                    // Set Last Cell Index
                    cellEditor.lastCellIndex = cellIndex;

                    // Set Animatin Durations
                    cellEditor.xAnimDuration = TEStyle.defaultDurationShort;
                    cellEditor.yAnimDuration = TEStyle.defaultDurationShort;
                    cellEditor.widthAnimDuration = TEStyle.defaultDurationShort;

                    // Set Cell Editor X Position
                    cellEditor.x = Qt.binding(function() {
                        // Check Current Item
                        if (packageListView.currentItem) {
                            return packageListView.currentItem.getCellPosX(cellIndex) + TEStyle.defaultMargin;
                        }

                        return 0;
                    });

                    // Set Cell Editor X Position
                    cellEditor.y = Qt.binding(function() {
                        // Check Current Item
                        if (packageListView.currentItem) {
                            return packageListView.currentItem.y - packageListView.contentY;
                        }

                        return 0;
                    });

                    cellEditor.width = Qt.binding(function() {
                        // Check Current Item
                        if (packageListView.currentItem) {
                            // Get Cell
                            var currentCell = packageListView.currentItem.getCell(cellIndex);

                            return currentCell.width;
                        }

                        return 0;
                    });

                    // Set Height
                    cellEditor.height = TEStyle.listItemHeight;

                    // Set Cell Editor Text
                    cellEditor.setText(mainController.currentItem ? mainController.currentItem.getTextData(cellIndex) : "");

                    // Set Cell Visible
                    cellEditor.cellVisible = Qt.binding(function() {
                        // Check Current Item
                        if (packageListView.currentItem) {
                            var currentCell = packageListView.currentItem.getCell(cellIndex);
                            return currentCell.visible;
                        }

                        return false;
                    });

                    // Show Cell Editor
                    cellEditor.show();
                }
            }
        }
    }

    // Scroll Indicator - Vertical
    TEScrollIndicator {
        id: verticalScrollIndicator

        anchors.right: horizontalFlickable.right
        anchors.top: horizontalFlickable.top
        anchors.topMargin: packageListHeader.height

        vertical: true

        viewSize: packageListView.height

        viewContentSize: {
            // Check Current Package
            if (mainController.currentPackage) {
                return packageListView.count * TEStyle.listItemHeight + mainController.currentPackage.selectedGroups.length * TEStyle.listItemHeight
            }

            return 0;
        }

        viewContentPos: packageListView.contentY
    }

    // Scroll Indicator - Horizontal
    TEScrollIndicator {
        id: horizontalScrollIndicator

        anchors.left: horizontalFlickable.left
        anchors.bottom: horizontalFlickable.bottom

        vertical: false

        viewSize: horizontalFlickable.width
        viewContentSize: horizontalFlickable.contentWidth
        viewContentPos: horizontalFlickable.contentX
    }

    // Busy Indicator for Checking
    TEBusyIndicator {
        anchors.right: parent.right
        anchors.rightMargin: TEStyle.defaultMargin * 4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: TEStyle.defaultMargin * 4
        opacity: mainController.checkingActive ? 1.0 : 0.0
    }

    // Filter Editor Box
    TEFilterEditor {
        id: filterEditor

        showChecked: packageListView.filterModel.showChecked
        showDraft: packageListView.filterModel.showDraft
        showWarning: packageListView.filterModel.showWarning

        onFilterTextChanged: {
            // Check Current Package
            if (mainController.currentPackage) {
                // Set Filter Text
                mainController.currentPackage.filterText = filterEditor.filterText;
            }

            // Check Filter Text Length
            if (filterText.length > 0) {
                //console.log("TEMainView.filterEditor.onFilterTextChanged - currentIndex: " + packageListView.currentIndex);
                // Reset Current Index For some reason if it's not reset some items/delegates not displayed!!!
                packageListView.currentIndex = -1;
                // Restart Filter Timer
                currentIndexFilterTimer.restart();
            } else {
                // Check Current Index
                if (packageListView.currentIndex === -1) {
                    // Set Current Index
                    packageListView.currentIndex = 0;
                }
            }
        }

        onShowCheckedClicked: {
            // Toggle Show Checked
            packageListView.filterModel.showChecked = !packageListView.filterModel.showChecked;
        }

        onShowDraftClicked: {
            // Toggle Show Draft
            packageListView.filterModel.showDraft = !packageListView.filterModel.showDraft;
        }

        onShowWarningClicked: {
            // Toggle Show Warning
            packageListView.filterModel.showWarning = !packageListView.filterModel.showWarning;
        }

        onStateChanged: {
            // Check State
            if (filterEditor.state === "hidden") {
                // Request Focus
                mainViewRoot.focus = true;
            }
        }
    }

    // Current Index Filter Timer
    Timer {
        id: currentIndexFilterTimer
        repeat: false
        interval: TEStyle.defaultDurationNormal
        onTriggered: {
            console.log("currentIndexFilterTimer.onTriggered - currentIndex: " + packageListView.currentIndex);
            // Check Current Index
            if (packageListView.currentIndex === -1 && packageListView.count > 0) {
                // Set Current Index
                packageListView.currentIndex = 0;
            }

            // Force Layout
            packageListView.forceLayout();
        }
    }

    // Dismiss Popup Mouse Area
    MouseArea {
        anchors.fill: parent
        visible: itemPopup.visible || headerPopup.visible

        onClicked: {
            // Dismiss Popup
            itemPopup.dismiss();
            headerPopup.dismiss();
        }
    }

    // Translation Item Popup
    TEPopup {
        id: itemPopup

        // Model
        model: mainViewRoot.popupModel

        onItemClicked: {
            console.log("itemPopup.onItemClicked - index: " + index);

            // Handle Popup Item Selection
            handlePopupItemSelection(index);

            // Request Focus
            mainViewRoot.focus = true;

            // Reset Single Selection Index
            mainController.currentPackage.singleSelectionIndex = -1;
        }

        onPopupDismissed: {
            // Request Focus
            mainViewRoot.focus = true;

            // Reset Single Selection Index
            mainController.currentPackage.singleSelectionIndex = -1;
        }
    }

    // Header Popup
    TEPopup {
        id: headerPopup

        model: mainController.currentPackage ? mainController.currentPackage.languages : 0

        itemsSpacing: TEStyle.defaultSpacing

        popupItemDelegate: TELanguagePopupItem {
            text: mainController.getLanguageName(modelData)
            source: mainController.getLanguageFlag(modelData)

            checked: {
                // Check Current Package
                if (mainController.currentPackage) {
                    return mainController.currentPackage.selectedLanguages.indexOf(modelData) !== -1;
                }

                return false;
            }

            onClicked: {
                // Check Current Package
                if (mainController.currentPackage) {
                    // Set Selected Language
                    mainController.currentPackage.setSelectedLanguage(modelData, !checked);
                }
            }

            onContainsMouseChanged: {
                // Check Contains Mouse
                if (containsMouse) {
                    // Set Current Index
                    headerPopup.currentIndex = index;
                }
            }
        }
    }

    // ...
}
