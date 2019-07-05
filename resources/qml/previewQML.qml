import QtQuick 2.0

import customcomponents 1.0

import "qrc:/resources/qml/style"

Item {
    id: previewRoot

    // Current Translation Item
    //property QtObject currentTranslation: mainController.currentItem

    // Selected Item Language
    property string selectedLanguage: mainController.selectedItemLanguage

    // Last Font name
    property string lastFontName: ""

    // Font Loader
    property FontLoader fontLoader: FontLoader {
//        onStatusChanged: {
//            console.log("previewRoot.fontLoader.onStatusChanged - status: " + status);
//        }

//        onSourceChanged: {
//            console.log("previewRoot.fontLoader.onSourceChanged - source: " + source);
//        }

        onNameChanged: {
            console.log("previewRoot.fontLoader.onNameChanged - name: " + name);

            // Check Name
            if (name !== "") {
                // Set Font Family
                previewTextLabel.font.family = name;
            }
        }
    }

    // Current Item Connections
    property Connections mainControllerConnections: Connections {
        target: mainController

        // On Current Item Changed Slot
        onCurrentItemChanged: {
            //console.log("previewRoot.mainControllerConnections.onCurrentItemChanged - currentItem: " + mainController.currentItem);

            // Check Current Item
            if (mainController.currentItem) {
                // Set Preview Text Label Text
                previewTextLabel.text = mainController.currentItem.getTranslation(mainController.selectedItemLanguage);

                // Check If Checking Mode Is On
                if (mainController.checkingActive) {
                    // Reset Current Item
                    fullCheck.resetCurrentItem();

                    // ...

//                    // Restart Timer
//                    fullCheck.restartItemCheckTimer();

//                    // Launch Single Check
//                    fullCheck.launchSingleCheck();
                }
            } else {
                // Reset Preview Text Label Text
                previewTextLabel.text = "No Current Item";
            }
        }

        // On Selected Item Language Changed Slot
        onSelectedItemLanguageChanged: {
            //console.log("previewRoot.mainControllerConnections.onSelectedItemLanguageChanged - selectedItemLanguage: " + mainController.selectedItemLanguage);

            // Check Current Item
            if (mainController.currentItem) {
                // Set Preview Text Label Text
                previewTextLabel.text = mainController.currentItem.getTranslation(mainController.selectedItemLanguage);
            } else {
                // Reset Preview Text Label Text
                previewTextLabel.text = "No Current Item";
            }
        }

        // On Current Layout Changed Slot
        onCurrentLayoutChanged: {
            // Check Current Layout
            if (mainController.currentLayout) {
                //console.log("previewRoot.mainControllerConnections.onCurrentLayoutChanged");

                // Load Layout Font
                loadLayoutFont();

                // Check If Checking Mode Is On
                if (mainController.checkingActive) {
                    // Check Current Item State
                    if (mainController.currentItem && mainController.currentItem.textState === "checked") {
                        // Restart Next Item Delay Timer
                        fullCheck.restartNextItemDelayTimer();

                        // Go To Next Translation Item
                        //fullCheck.gotoNextTranslationItem();

                        return;
                    }

                    // Reset Language Index
                    fullCheck.languageIndex = -1;
                    fullCheck.languageIndex = 0;
                    // Restart Timer
                    fullCheck.restartItemCheckTimer();
                }
            }
        }

        // On Checking Started Slot
        onCheckingStarted: {
            // Check Current Package
            if (mainController.currentPackage) {
                console.log("previewRoot.mainControllerConnections.onCheckingStarted - rowCount: " + mainController.currentPackage.rowCount());

                // Reset Single Check
                fullCheck.singleCheck = false;

                // Init & Run Full Check
                fullCheck.launchFullCheck();

                // ...
            }
        }

        // On Single Check Started Slot
        onSingleCheckStarted: {
            // Check Current Item
            if (mainController.currentItem) {
                console.log("previewRoot.mainControllerConnections.onSingleCheckStarted - currentIndex: " + mainController.currentIndex);

                // Set Single Check
                fullCheck.singleCheck = true;

                // Init & Launch Single Check
                fullCheck.launchItemCheck();

                // ...

            }
        }

        // On Checking Aborted Signal
        onCheckingAborted: {
            console.log("previewRoot.mainControllerConnections.onCheckingAborted");
            // Stop Item Check Timer
            itemCheckTimer.running = false;

            // Check Current Translation Item
            if (mainController.currentItem) {
                // Reset State to Draft
                mainController.currentItem.textState = "Draft";
            }

            // ...
        }

        // On Checking Index Changed Slot
        onCheckIndexChanged: {
            console.log("previewRoot.mainControllerConnections.onCheckIndexChanged - checkIndex: " + mainController.checkIndex);

            // Select Translation Item
            mainController.selectTranslationItem(mainController.checkIndex);

            // Launch Translation Item Check
            fullCheck.launchItemCheck();

            // ...
        }
    }

    // Current Item Connections
    property Connections currentItemConnections: Connections {
        target: mainController.currentItem

        onTranslationChanged: {
            //console.log("previewRoot.currentItemConnections.onTranslationChanged - aLanguage: " + aLanguage + " - aTranslation: " + aTranslation);

            // Update Text
            previewTextLabel.text = Qt.binding(function() {
                // Check Current Item
                if (mainController.currentItem) {
                    return mainController.currentItem.getTranslation(mainController.selectedItemLanguage);
                }

                return "No Current Item";
            });

            // ...
        }

        onCurrentLayoutChanged: {
            //console.log("previewRoot.currentItemConnections.onCurrentLayoutChanged");
            // Set Current Layout
            mainController.setCurrentLayout(aCurrentLayout);
        }
    }

    // Current Item Connections
    property Connections currentLayoutConnections: Connections {
        target: mainController.currentLayout

        onFontNameChanged: {
            //console.log("previewRoot.currentLayoutConnections.onFontNameChanged - newFontName: " + newFontName);

            // Load Default Layout Font - Will Trigger Boundary Check
            loadFont(mainController.currentLayout.fontName);
        }

        onLayoutChanged: {
            //console.log("previewRoot.currentLayoutConnections.onLayoutChanged");

            // Check Current Item
            if (mainController.currentItem) {
                // Set Item State
                mainController.currentItem.textState = "draft";
            }

            // Reset Alternative Font Index
            mainController.currentLayout.setAlternativeFontIndex(previewRoot.selectedLanguage, -1);

            // Load Layout Font
            loadLayoutFont();

            // Restart Boundary Check Timer
            boundaryCheckTimer.restart();
        }

        // Alternate Fonts Changed
        onAltFontsChanged: {
            //console.log("previewRoot.currentLayoutConnections.onAltFontsChanged");

            // ...
        }
    }

    // On Selected Language Changed Slot
    onSelectedLanguageChanged: {
        //console.log("previewRoot.onSelectedLanguageChanged - selectedLanguage: " + previewRoot.selectedLanguage);

        // Load Layout Font
        loadLayoutFont();

//        // Restart Boundary Check Timer
//        boundaryCheckTimer.restart();

        // ...
    }

    // Normalize Path
    function normalizePath(path) {
        if (path[0] !== '/')
            return "/" + path;
        return path;
    }

    // Load Font
    function loadFont(fontName) {
        // Check Last Font Name
        if (lastFontName !== fontName && fontName.length !== 0) {
            // Set Last Font Name
            lastFontName = fontName;
            // Init Font Absolute Path
            var fontAbsolutePath = "file://" + normalizePath(mainController.currentPackage.fontsDir) + "/" + lastFontName;

            //console.log("previewRoot.loadFont - fontAbsolutePath: " + fontAbsolutePath);

            // Set Font Loader Source
            fontLoader.source = fontAbsolutePath;
        }
    }

    // Load Layout Font
    function loadLayoutFont() {
        // Check Current Layout
        if (mainController.currentPackage && mainController.currentLayout && previewRoot.selectedLanguage !== "") {
            // Init Font Name
            var fontName = mainController.currentLayout.fontName;

            //console.log("previewQML.loadLayoutFont - shrinkable: " + mainController.currentLayout.shrinkable + " - altFonts.length: " + mainController.currentLayout.altFonts.length);

            // Check If Layout Shrinkable
            if (mainController.currentLayout.shrinkable && mainController.currentLayout.altFonts.length > 0) {
                // Get Alternative Font Index
                var afIndex = mainController.currentLayout.getAlternativeFontIndex(previewRoot.selectedLanguage);

                //console.log("previewQML.loadLayoutFont - previewRoot.selectedLanguage: " + previewRoot.selectedLanguage + " - afIndex: " + afIndex);

                // Check Alternative Font Index
                if ((afIndex >= 0) && (afIndex < mainController.currentLayout.altFonts.length)) {
                    // Update Font Name
                    fontName = mainController.currentLayout.altFonts[afIndex];

                    //console.log("previewQML.loadLayoutFont - fontName: " + fontName);
                }
            }

            // Check Font Name
            if (fontName.length > 0) {
                // Load Font
                loadFont(fontName);
            }
        }
    }

    // Check Boundary And Load Alternative Font If Possible AND/OR Necessary
    function checkBoundary() {
        // Stop Item Check Timer
        itemCheckTimer.running = false;

        // Check Current Layout
        if (mainController.currentItem && mainController.currentLayout) {
            console.log("previewQML.checkBoundary");

            // Check If Out Of Bounds
            if (boundaryRect.width < previewTextLabel.contentWidth || boundaryRect.height < previewTextLabel.contentHeight) {
                // Check If Shrinkable
                if (mainController.currentLayout.shrinkable && mainController.currentLayout.altFonts.length > 0) {
                    // Get Alternative Font Index
                    var afIndex = mainController.currentLayout.getAlternativeFontIndex(previewRoot.selectedLanguage);

                    // Check Alternative Font Index
                    if (afIndex < mainController.currentLayout.altFonts.length) {
                        // Increase Alternative Font Index
                        afIndex++;
                        // Set Alternative Font Index
                        mainController.currentLayout.setAlternativeFontIndex(previewRoot.selectedLanguage, afIndex);

                        // Load Layout Font
                        loadLayoutFont();

                        // This will trigger Re-Check Boundary

                        return;
                    }
                }

                // Load Layout Font
                loadLayoutFont();
                // Mark Language Unfit
                mainController.currentItem.markItemLanguageUnfit(previewRoot.selectedLanguage);
            }
        }

        // Check If Checking Mode Active
        if (mainController.checkingActive) {
            // Restart Item Check Timer
            fullCheck.restartItemCheckTimer();
        }
    }

    // Background
    TEBackground { }

    // Boundary Rect
    Rectangle {
        id: boundaryRect

        width: {
            // Check Current Layout
            if (mainController.currentItem && mainController.currentLayout && mainController.currentLayout.maxWidth !== -1) {
                return mainController.currentLayout.maxWidth;
            }

            return Math.max(parent.width, previewTextLabel.width);
        }

        height: {
            // Check Current Layout
            if (mainController.currentItem && mainController.currentLayout && mainController.currentLayout.maxHeight !== -1) {
                return mainController.currentLayout.maxHeight;
            }

            return Math.max(parent.height, previewTextLabel.height);
        }

        anchors.centerIn: parent

        // Out Of Bounds
        property bool outOfBounds: {
            // Check Text Length
            if (previewTextLabel.text.length === 0) {
                return true;
            }

            // Check Label Size
            if (boundaryRect.width < previewTextLabel.contentWidth || boundaryRect.height < previewTextLabel.contentHeight) {
                return true;
            }

            // Check Current Layout Max Length
            if (mainController.currentLayout && mainController.currentLayout.maxLength > 0 &&
                    previewTextLabel.text.length > mainController.currentLayout.maxLength) {
                return true;
            }

            return false;
        }

        // Shrinked Fit
        property bool shrinkedFit: {
            // Check Current Layout
            if (mainController.currentLayout) {
                // Get Current Layout Font Name
                var clFontName = mainController.currentLayout.fontName.substring(0, mainController.currentLayout.fontName.lastIndexOf("."));

                //console.log("#### clFontName: " + clFontName);

                return (fontLoader.name !== clFontName);
            }

            return false;
        }

        border.color: outOfBounds ? "Red" : shrinkedFit ? "Purple" : "Green"

        color: outOfBounds ? "#22770000" : "#22007700"
    }

    // No Translation Label
    TELabel {
        anchors.centerIn: parent
        opacity: 0.5
        visible: previewTextLabel.text.length === 0
        text: "No Translation Specified"
    }

    // Preview Text Label
    Text {
        id: previewTextLabel

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        width: {
            // Check Current Layout
            if (mainController.currentLayout) {
                // Check Max Width
                if (mainController.currentLayout.maxWidth === -1) {
                    return undefined;
                }

                return mainController.currentLayout.maxWidth;
            }

            return 0;
        }

        height: {
            // Check Current Layout
            if (mainController.currentLayout) {
                // Check Max Height
                if (mainController.currentLayout.maxHeight === -1) {
                    return undefined;
                }

                return mainController.currentLayout.maxHeight;
            }

            return 0;
        }

        anchors.centerIn: parent

        color: TEStyle.defaultColorText

        //font.family: fontLoader.name

        font.pixelSize: {
            // Check Current Layout
            if (mainController.currentLayout) {
                return mainController.currentLayout.fontSize;
            }

            if (mainController.currentPackage) {
                return mainController.currentPackage.defaultFontSize;
            }

            return 16;
        }

        textFormat: Text.PlainText

        wrapMode: {
            // Check Current Layout
            if (mainController.currentLayout) {
                switch (mainController.currentLayout.wrapType) {
                    case 1: return Text.WordWrap;
                    case 2: return Text.WrapAtWordBoundaryOrAnywhere;
                    case 3: return Text.WrapAnywhere;
                }
            }

            return Text.NoWrap;
        }

        elide: {
            // Check Current Layout
            if (mainController.currentLayout) {
                switch (mainController.currentLayout.elideType) {
                    case 1: return Text.ElideLeft;
                    case 2: return Text.ElideMiddle;
                    case 3: return Text.ElideRight;
                }
            }

            return Text.ElideNone;
        }

        maximumLineCount: {
            // Check Current Layout
            if (mainController.currentLayout) {
                return mainController.currentLayout.maxLines;
            }

            return 1;
        }
/*
        text: {

//            // Check Current Item
//            if (mainController.currentItem) {
//                console.log("#########################################################################");
//                console.log("#### previewQML.previewTextLabel.text: " + mainController.currentItem);
//                console.log("#########################################################################");

//                var translatedText = mainController.currentItem.getTranslation(mainController.selectedItemLanguage);
//                //console.log("previewQML.previewTextLabel.text - translatedText: " + translatedText);
//                return translatedText;
//            }

            return "No Current Item";
        }
*/

//        onTextChanged: {
//            // Check Current Item & Layout
//            if (mainController.currentItem && mainController.currentLayout) {
//                //console.log("previewQML.previewTextLabel.onTextChanged - text: " + text);

//                // ...
//            }
//        }

        onContentSizeChanged: {
            // Check Content Width & Height
            if (contentWidth > 0 && contentHeight > 0) {
                // Check Boundary Rect
                if (boundaryRect.width < previewTextLabel.contentWidth || boundaryRect.height < previewTextLabel.contentHeight) {
                    //console.log("previewQML.previewTextLabel.onContentSizeChanged - contentWidth: " + contentWidth + " - contentHeight: " + contentHeight + " - OUT OF BOUNDS!");

                    // Restart Boundary Check Timer
                    boundaryCheckTimer.restart();
                }
            }
        }

        // Actual Text Layout Border Rectangle
        Rectangle {
            id: layoutBorderRect

            anchors.fill: parent

            border.color: "Cyan"
            color: "transparent"

            opacity: 0.2
        }
    }

    // Actual Font Name Label
    TELabel {
        id: actualFontNameLabel

        anchors.left: parent.left
        anchors.leftMargin: TEStyle.defaultMargin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: TEStyle.defaultMargin

        opacity: 0.7
        visible: previewTextLabel.text.length > 0

        text: fontLoader.name
    }

    // Actual Size Label
    TELabel {
        id: actualSizeLabel

        anchors.right: parent.right
        anchors.rightMargin: TEStyle.defaultMargin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: TEStyle.defaultMargin

        opacity: 0.7
        visible: previewTextLabel.text.length > 0

        text: "[" + Math.round(previewTextLabel.width) + "x" + Math.round(previewTextLabel.height) + "]"
    }

    // Full Check Test Item
    Item {
        id: fullCheck

        // Single Check Flag
        property bool singleCheck: false
        // Translation Count
        property int translationCount: mainController.currentPackage ? mainController.currentPackage.count : 0
        // Default Wait
        readonly property int defaultWait: TEStyle.defaultDurationShort * 0.5
        // Current Translation Item Languages
        property var translationLanguages: mainController.currentPackage ? mainController.currentPackage.languages : ""
        // Language Index
        property int languageIndex: -1

        // On Language Index Changed Slot
        onLanguageIndexChanged: {
            // Check Language Index
            if (languageIndex !== -1) {
                // Set Selected Language Index
                mainController.setSelectedTranslationLanguage(languageIndex, true);

                // Check If Checking Mode Is On
                if (mainController.checkingActive) {
                    // Restart Timer
                    fullCheck.restartItemCheckTimer();
                }
            }
        }

        // Init & Run Translation Item Check
        function launchItemCheck() {
            console.log("fullCheck.launchItemCheck - singleCheck: " + fullCheck.singleCheck);

            // Reset Language Index
            fullCheck.languageIndex = -1;
            fullCheck.languageIndex = 0;

            // Reset Current Item
            fullCheck.resetCurrentItem();

            // Restart Timer
            fullCheck.restartItemCheckTimer();

            // ...
        }

        // Init & Run Full Check
        function launchFullCheck() {
            console.log("fullCheck.launchFullCheck");

//            // Reset Language Index
//            fullCheck.languageIndex = -1;
//            fullCheck.languageIndex = 0;

            // Reset Check Index
            mainController.checkIndex = -1;

            // Check Current Package
            if (mainController.currentPackage) {
                // Check Selection Count
                if (mainController.currentPackage.selectedItemCount > 0) {
                    // Go To Next Selected Item Index
                    gotoNextSelectedItem();
                } else {
                    // Reset Check Index
                    mainController.checkIndex = 0;
                }
            }
        }

        // Reset Current Translation Item for Checking
        function resetCurrentItem() {
            // Check Current Translation Item
            if (mainController.currentItem && (mainController.currentItem.textState !== "checked")) {
                console.log("fullCheck.resetCurrentItem - checkIndex: " + mainController.checkIndex);
                // Reset Layout Index
                mainController.currentItem.layoutIndex = -1;
                mainController.currentItem.layoutIndex = 0;
                // Set State
                mainController.currentItem.textState = "checking";
                // Clear Unfit Item Languages
                mainController.currentItem.clearUnfitLanguages();

                //console.log("fullCheck.resetCurrentItem - translationLanguages: " + fullCheck.translationLanguages);
            }
        }

        // Check Current Translation Text
        function checkCurrentTranslationText() {
            // Check Current Translation Item
            if (mainController.currentItem) {
                console.log("fullCheck.checkCurrentTranslationText - selectedLanguage: " + previewRoot.selectedLanguage);

                // Check Boundary Rect
                if (boundaryRect.outOfBounds) {
                    // Set State
                    mainController.currentItem.textState = "Warning";
                    // Mark Translation Item Language Unfit
                    mainController.currentItem.markItemLanguageUnfit(previewRoot.selectedLanguage);
                }

                // Check If Checking Mode Is On
                if (mainController.checkingActive) {
                    // Go To Next Language
                    gotoNextLanguage();
                }
            }
        }

        // Go To Next Language
        function gotoNextLanguage() {
            // Check Current Item
            if (mainController.currentItem) {
                console.log("fullCheck.gotoNextLanguage - languageIndex: " + fullCheck.languageIndex);
                // Check Current Language Index
                if (fullCheck.languageIndex < fullCheck.translationLanguages.length - 1) {
                    // Increase Language Index
                    fullCheck.languageIndex++;
                } else {
                    // Check If Checking Mode Is On
                    if (mainController.checkingActive) {
                        // Goto Next Layout
                        gotoNextLayout();
                    }
                }
            }
        }

        // Go To Next Layout
        function gotoNextLayout() {
            // Check Current Item
            if (mainController.currentItem) {
                console.log("fullCheck.gotoNextLayout - layoutIndex: " + mainController.currentItem.layoutIndex);
                // Chek Layout Index
                if (mainController.currentItem.layoutIndex < mainController.currentItem.layoutCount() - 1) {
                    // Go To Next Layout
                    mainController.currentItem.nextLayout();
                } else {
                    // Check If Checking Mode Is On
                    if (mainController.checkingActive) {
                        // Reset Layout Index
                        mainController.currentItem.layoutIndex = 0;

                        // Stop Item Check Timer
                        itemCheckTimer.running = false;

                        console.log("fullCheck.gotoNextLayout - unfitLanguages.length: " + mainController.currentItem.unfitLanguages.length);

                        // Check Unfit Languages
                        if (mainController.currentItem.unfitLanguages.length === 0) {
                            // Set Text State
                            mainController.currentItem.textState = "Checked";
                        }

                        // Go To Next Translation Item
                        gotoNextTranslationItem();
                    }
                }
            }
        }

        // Go To Next Selected Translation Item
        function gotoNextSelectedItem() {
            // Check Current Package
            if (mainController.currentPackage) {
                // Iterate Through Items
                for (var i=mainController.checkIndex + 1; i<fullCheck.translationCount; i++) {
                    // Get Item
                    var trItem = mainController.currentPackage.getItem(i);
                    // Check Selected State
                    if (trItem.selected) {
                        // Set Checking Index
                        mainController.checkIndex = i;

                        return;
                    }
                }

                // Finish Checking All Translations
                mainController.finishCheckingAllTranslations(fullCheck.singleCheck);
            }
        }

        // Go To Next Translation Item
        function gotoNextTranslationItem() {
            // Check Current Package
            if (mainController.currentPackage) {
                // Refresh Row
                mainController.currentPackage.refresh(mainController.checkIndex);
            }

            console.log("fullCheck.gotoNextTranslationItem - mainController.checkIndex: " + mainController.checkIndex + " - fullCheck.translationCount: " + fullCheck.translationCount);

            // Check Count
            if (mainController.checkIndex < fullCheck.translationCount - 1 && !fullCheck.singleCheck) {
                // Check Selected Item Count
                if (mainController.currentPackage.selectedItemCount > 0) {
                    // Go To Next Selected Translation Item
                    gotoNextSelectedItem();
                } else {
                    // Increase Check Index - Will Trigger New Translation Item Load
                    mainController.checkIndex++;
                }
            } else {
                // Finish Checking All Translations
                mainController.finishCheckingAllTranslations(fullCheck.singleCheck);
            }
        }

        // Restart Item Check Timer
        function restartItemCheckTimer() {
            // Restart Item Check Timer
            itemCheckTimer.restart();
        }

        // Restart Next Item Timer
        function restartNextItemDelayTimer() {
            // Restart Next Item Delay Timer
            nextItemDelayTimer.restart();
        }

        // Next Item Delay Timer
        Timer {
            id: nextItemDelayTimer
            interval: fullCheck.defaultWait
            repeat: false

            onTriggered: {
                // Go To Next Translation Item
                fullCheck.gotoNextTranslationItem();
            }
        }

        // Item Check Timer
        Timer {
            id: itemCheckTimer
            interval: fullCheck.defaultWait
            repeat: false

            onTriggered: {
                // Check Current Translation Text
                fullCheck.checkCurrentTranslationText();
            }
        }

        // Boundary Check Timer
        Timer {
            id: boundaryCheckTimer
            interval: 5
            repeat: false
            onTriggered: {
                // Check Boundary
                checkBoundary();

                // Check If Checking Mode Is On
                if (mainController.checkingActive) {
                    // Restart Check Timer
                    fullCheck.restartItemCheckTimer();
                }
            }
        }
    }
}
