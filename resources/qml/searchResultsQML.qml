import QtQuick 2.0

import "qrc:/resources/qml/style"

Item {
    id: searchResultsRoot

    TEBackground { }

    // No Results Label
    TELabel {
        anchors.centerIn: parent
        visible: resultsListView.count === 0
        text: "No Results"
    }

    // Header Row
    Row {
        id: headerRow

        height: TEStyle.defaultHeaderHeight

        spacing: TEStyle.defaultSpacing
        visible: resultsListView.count > 0

        TEHeaderItem {
            id: headerIndex
            width: 84

            headerItemIndex: 1000
            storeSize: false
            textAlignment: Text.AlignHCenter
            title: "Index"
        }

        TEHeaderItem {
            id: headerID
            width: 192

            headerItemIndex: 1001
            storeSize: false
            title: "Label ID"
        }

        TEHeaderItem {
            id: headerOriginal
            width: 192

            headerItemIndex: 1002
            storeSize: false
            title: "Original Text"
        }

        TEHeaderItem {
            id: headerMatches
            width: Math.max(0, searchResultsRoot.width - headerIndex.width - headerID.width - headerOriginal.width)

            headerItemIndex: 1003
            storeSize: false
            title: "Matches"
        }
    }

    // Results List View
    ListView {
        id: resultsListView

        width: headerRow.width

        anchors.left: parent.left
        anchors.top: headerRow.bottom
        anchors.bottom: parent.bottom

        visible: resultsListView.count > 0
        clip: true
        highlightFollowsCurrentItem: true

        model: searchResultModel

        currentIndex: -1

        delegate: MouseArea {
            width: resultsListView.width
            height: TEStyle.listItemHeight

            // Row Background
            Rectangle {
                anchors.fill: parent
                color: (((index % 2) > 0) && preferences.alternateRowColors) ? TEStyle.defaultColorRowAlternate : "Transparent";
            }

            // Item Row
            Row {
                height: parent.height
                spacing: headerRow.spacing

                TELabel {
                    id: labelIndex
                    width: headerIndex.width
                    height: parent.height
                    text: model.sourceIndex + 1
                }

                TELabel {
                    id: labelID
                    width: headerID.width
                    height: parent.height
                    horizontalAlignment: Text.AlignLeft
                    text: model.sourceLabelID
                }

                TELabel {
                    id: labelOriginal
                    width: headerOriginal.width
                    height: parent.height
                    horizontalAlignment: Text.AlignLeft
                    text: model.sourceOriginalText
                }

                TELabel {
                    id: labelMatches
                    width: headerMatches.width
                    height: parent.height
                    horizontalAlignment: Text.AlignLeft
                    text: model.sourceMatches
                }
            }

            onClicked: {
                // Set Current Index
                resultsListView.currentIndex = index;
            }

            onDoubleClicked: {
                // Emit Result Item Selected Signal
                searchController.resultItemSelected(model.sourceIndex);
            }
        }

        highlightMoveDuration: TEStyle.defaultDurationShort

        highlight: Rectangle {
            color: TEStyle.defaultColorListHighlight
        }

        onCountChanged: {
            //console.log("resultsListView.onCountChanged - count: " + count);

            // Check Count
            if (count > 0) {
                // Check Current Index
                if (currentIndex === -1) {
                    // Set Current Index
                    currentIndex = 0;
                }
            } else {
                // Reset Current Index
                currentIndex = -1;
            }
        }
    }

    // Scroll Indicator - Vertical
    TEScrollIndicator {
        id: verticalScrollIndicator

        anchors.right: parent.right
        anchors.rightMargin: TEStyle.defaultMargin
        anchors.top: resultsListView.top

        vertical: true

        viewSize: resultsListView.height
        viewContentSize: resultsListView.contentHeight
        viewContentPos: resultsListView.contentY
    }
}
