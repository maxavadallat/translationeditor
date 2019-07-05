import QtQuick 2.0

import "qrc:/resources/qml/style"

Row {
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
            columnIndex: 3 + index

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

            onCellPressed: translationItemRoot.cellPressed(cell, columnIndex, modifiers);
            onCellClicked: translationItemRoot.cellClicked(cell, columnIndex, modifiers);
            onCellDoubleClicked: translationItemRoot.cellDoubleClicked(cell, columnIndex);
        }
    }
}
