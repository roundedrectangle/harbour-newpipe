import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.newpipe.extractor 1.0
import "../components"

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
            searchModel.search(extractor, searchString);
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

        onContentYChanged: {
            var pos = contentHeight + originY - height - contentY;
            if ((pos < height) && !searchModel.loading && searchModel.more && searchModel.nextPage) {
                searchModel.searchMore(extractor, searchString);
            }
        }

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

                SearchThumbnail {
                    id: thumbnail
                    infoType: model.infoType
                    source: model.thumbnail
                    width: Theme.iconSizeLarge * iconScale
                    height: Theme.iconSizeMedium * iconScale
                }

                Column {
                    width: parent.width - thumbnail.width - Theme.paddingLarge
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        color: delegate.pressed ? Theme.highlightColor : Theme.primaryColor
                        textFormat: Text.PlainText
                        text: model.name
                        width: parent.width
                        truncationMode: TruncationMode.Fade
                        focus: false
                    }

                    Label {
                        color: delegate.pressed ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        textFormat: Text.PlainText
                        font.pixelSize: Theme.fontSizeExtraSmall
                        text: model.infoRow || ""
                        truncationMode: TruncationMode.Fade
                        focus: false
                        visible: !!model.infoRow
                    }
                }
            }

            onClicked: {
                switch (model.infoType) {
                    case SearchItem.Channel:
                        pageStack.push(Qt.resolvedUrl("ChannelPage.qml"), {
                            name: model.name,
                            thumbnail: model.thumbnail,
                            url: model.url
                        });
                        break;
                    case SearchItem.Playlist:
                        // Do nothing
                        break;
                    case SearchItem.Playlist:
                        pageStack.push(Qt.resolvedUrl("VideoPage.qml"), {
                            name: model.name,
                            thumbnail: model.thumbnail,
                            url: model.url
                        });
                        break;
                    default:
                        console.log("Unknown search item type: " + model.infoType);
                        break;
                }
            }
        }
    }
}
