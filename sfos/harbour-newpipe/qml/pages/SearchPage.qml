import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.newpipe.extractor 1.0

Page {
    id: page

    property string searchString
    property string screenName
    property int totalitems: 0
    property bool busy: false
    property int displayCount: 0
    readonly property alias pendingSearch: searchTimer.running

    onSearchStringChanged: {
        searchTimer.restart()
    }

    Timer {
        id: searchTimer
        interval: 500
        repeat: false
        onTriggered: {
            extractor.search(searchString);
        }
    }

    SilicaListView {
        id: listView
        model: searchModel
        anchors.fill: parent

        onCurrentIndexChanged: {
            // This nasty hack prevents the currentIndex being set
            // away from -1
            // This avoids the virtual keyboard disappearing when the
            // search filter is changed
            listView.currentIndex = -1
            //console.log("CurrentIndex: " + currentIndex)
        }

        VerticalScrollDecorator {}

        header: Column {
            id: headerColumn
            width: page.width
            height: header.height + searchField.height

            PageHeader {
                id: header
                title: screenName
            }

            SearchField {
                id: searchField
                width: parent.width
                //% "Search"
                placeholderText: qsTrId("newpipe-proglist_search-placeholder")
                // Predictive text actually messes up the clear button so it only
                // works if there's more than one word (weird!), but predictive
                // is likely to be the more useful of the two, so I've left it on
                //inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                inputMethodHints: Qt.ImhNoAutoUppercase

                Binding {
                    target: page
                    property: "searchString"
                    value: searchField.text.toLowerCase().trim()
                }
            }
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            textFormat: Text.RichText
            //% "No entries"
            text: qsTrId("newpipe-proglist_search-no-entries")
            //% "Enter some text to search"
            hintText: qsTrId("newpipe-proglist_search-enter-some-text")
        }

        delegate: BackgroundItem {
            id: delegate
            focus: false
            height: thumbnail.height + (2 * Theme.paddingMedium)
            readonly property real iconScale: 1.5

            ListView.onAdd: AddAnimation {
                id: animadd
                target: delegate
            }
            ListView.onRemove: RemoveAnimation {
                id: animremove
                target: delegate
            }

            Row {
                x: Theme.paddingLarge
                y: Theme.paddingMedium
                width: parent.width - (2 * Theme.paddingLarge)
                height: thumbnail.height
                spacing: Theme.paddingLarge

                Image {
                    id: thumbnail
                    width: Theme.iconSizeLarge * iconScale
                    height: Theme.iconSizeMedium * iconScale
                    fillMode: Image.PreserveAspectCrop
                    verticalAlignment: Image.AlignVCenter
                    source: model.thumbnail
                }

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    color: Theme.primaryColor
                    textFormat: Text.StyledText
                    text: model.name
                    width: parent.width - thumbnail.width - Theme.paddingLarge
                    elide: Text.ElideRight
                    focus: false
                }
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("VideoPage.qml"), {
                    name: model.name,
                    thumbnail: model.thumbnail,
                    url: model.url
                });
            }
        }
    }
}
