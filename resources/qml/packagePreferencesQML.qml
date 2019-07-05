import QtQuick 2.0
import QtQuick.Layouts 1.3

import "qrc:/resources/qml/style"

Item {
    id: packagePreferencesRoot

    TEBackground { }

    ListView {
        id: languagesList

        anchors.fill: parent

        model: mainController.supportedLanguages

        delegate: TEListItemBase {
            id: delegateRoot

            width: languagesList.width
            height: TEStyle.listItemHeight

            Rectangle {
                anchors.fill: parent
                color: TEStyle.defaultColorListHighlight
                visible: delegateRoot.highlight
            }

            Row {
                anchors.left: parent.left
                anchors.leftMargin: TEStyle.defaultMargin

                anchors.verticalCenter: parent.verticalCenter

                spacing: TEStyle.defaultSpacing

                TEImage {
                    id: languageFlag

                    width: height
                    height: delegateRoot.height - TEStyle.defaultMargin * 2

                    anchors.verticalCenter: parent.verticalCenter

                    fillMode: Image.PreserveAspectFit

                    source: mainController.getLanguageFlag(mainController.supportedLanguages[index])
                }

                TELabel {
                    id: languageNameLabel

                    anchors.verticalCenter: parent.verticalCenter

                    text: mainController.getLanguageName(mainController.supportedLanguages[index])
                }
            }

            TECheckBox {
                id: selectionCheckBox

                anchors.right: parent.right
                anchors.rightMargin: TEStyle.defaultMargin
                anchors.verticalCenter: parent.verticalCenter

                enabled: false
                opacity: 1.0

                checked: {
                    // Check Current Package
                    if (packageController.currentPackage) {
                        // Check Selected Languages
                        if (packageController.currentPackage.languages.indexOf(mainController.supportedLanguages[index]) !== -1) {
                            return true;
                        }
                    }

                    return false;
                }
            }

            onClicked: {
                // Check Checked State
                if (selectionCheckBox.checked) {
                    // TODO: Ask For Confirmation!!!

                    // Remove Language
                    packageController.currentPackage.removeLanguage(mainController.supportedLanguages[index]);
                } else {
                    // Add Language
                    packageController.currentPackage.addLanguage(mainController.supportedLanguages[index]);
                }
            }
        }
    }
}
