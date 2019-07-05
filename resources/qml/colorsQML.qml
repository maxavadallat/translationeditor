import QtQuick 2.0

import customcomponents 1.0

import "qrc:/resources/qml/style"

Item {
    id: colorsPreviewRoot

    property int previewFontSize: cpFontSize
    property int previewRowHeight: cpRowSize

    property bool showAlternateRowColors: cpAltRows

    property string previewColorText: cpColorText
    property string previewColorBackground: cpColorBackground
    property string previewColorBorder: cpColorBorder
    property string previewColorRowAlt: cpColorRowAlt
    property string previewColorHeader: cpColorHeader
    property string previewColorHighlight: cpColorHighlight

    readonly property int numItems: 7

    Rectangle {
        anchors.fill: parent
        color: "Black"
    }

    // Background
    Rectangle {
        anchors.fill: parent
        //color: colorsPreviewRoot.previewColorBackground

        gradient: Gradient {
            GradientStop { position: 0.0; color: colorsPreviewRoot.previewColorBackground }
            GradientStop { position: 0.5; color: Qt.lighter(colorsPreviewRoot.previewColorBackground, 1.15) }
            GradientStop { position: 1.0; color: colorsPreviewRoot.previewColorBackground }
        }
    }

//    TELabel {
//        anchors.centerIn: parent
//        text: "Colors"
//        opacity: 0.1
//    }

    // Preview Column
    Column {
        // Header Row
        Row {
            Rectangle {
                id: headerItem1
                width: 54
                height: colorsPreviewRoot.previewRowHeight
                color: colorsPreviewRoot.previewColorHeader

                TELabel {
                    anchors.fill: parent
                    color: colorsPreviewRoot.previewColorText
                    font.pixelSize: colorsPreviewRoot.previewFontSize
                    text: "Index"
                }
            }

            Rectangle {
                width: TEStyle.defaultSpacing
                height: colorsPreviewRoot.previewRowHeight
                color: "orange"
            }

            Rectangle {
                id: headerItem2
                width: 148
                height: colorsPreviewRoot.previewRowHeight
                color: colorsPreviewRoot.previewColorHeader

                TELabel {
                    anchors.fill: parent
                    color: colorsPreviewRoot.previewColorText
                    font.pixelSize: colorsPreviewRoot.previewFontSize
                    text: "Column"
                }
            }

            Rectangle {
                width: TEStyle.defaultSpacing
                height: colorsPreviewRoot.previewRowHeight
                color: "orange"
            }
        }

        // Preview List
        ListView {
            id: colorsPreviewList

            width: colorsPreviewRoot.width
            height: colorsPreviewRoot.previewRowHeight * colorsPreviewRoot.numItems

            currentIndex: 4

            model: colorsPreviewRoot.numItems

            clip: true

            delegate: Rectangle {
                width: colorsPreviewRoot.width
                height: colorsPreviewRoot.previewRowHeight

                color: {
                    // Check Index
                    if (((index % 2) > 0) && colorsPreviewRoot.showAlternateRowColors) {
                        return colorsPreviewRoot.previewColorRowAlt;
                        //return "Lime";
                    }

                    return "Transparent";//colorsPreviewRoot.previewColorRow;
                }

                // Cells
                Row {
                    TELabel {
                        width: headerItem1.width
                        height: colorsPreviewRoot.previewRowHeight
                        color: colorsPreviewRoot.previewColorText
                        font.pixelSize: colorsPreviewRoot.previewFontSize
                        text: (index + 1)

//                        Rectangle {
//                            anchors.fill: parent
//                            color: "Transparent"
//                            border.color: colorsPreviewRoot.previewColorBorder
//                        }
                    }

                    Item {
                        width: TEStyle.defaultSpacing
                        height: colorsPreviewRoot.previewRowHeight
                    }

                    TELabel {
                        width: headerItem2.width
                        height: colorsPreviewRoot.previewRowHeight
                        color: colorsPreviewRoot.previewColorText
                        font.pixelSize: colorsPreviewRoot.previewFontSize
                        text: "Item " + (index + 1)

                        Rectangle {
                            anchors.fill: parent
                            radius: TEStyle.defaultRadius
                            color: "Transparent"
                            border.color: colorsPreviewRoot.previewColorBorder
                        }
                    }

                    Item {
                        width: TEStyle.defaultSpacing
                        height: colorsPreviewRoot.previewRowHeight
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: colorsPreviewList.currentIndex = index;
                }
            }

            highlight: Rectangle {
                width: colorsPreviewRoot.width
                height: colorsPreviewRoot.previewRowHeight
                color: colorsPreviewRoot.previewColorHighlight
            }
        }
    }
}
