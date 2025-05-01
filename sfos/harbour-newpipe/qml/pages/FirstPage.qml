import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    property string searchString
    property bool tv
    property string screenName
    property int totalitems: 0
    property bool busy: false
    property int displayCount: 0

    onSearchStringChanged: {
        (tv ? programmestv : programmesradio).setFilterFixedString(searchString)
        totalitems = (tv ? programmestv : programmesradio).sourceModel.rowCount()
    }

    SilicaListView {
        id: listView
        model: null
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
                placeholderText: qsTrId("newpipe-proglist_search_placeholder")
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
            text: "Placeholder"
            hintText: "Placeholder hint"
        }

        delegate: BackgroundItem {
            id: delegate
            focus: false

            ListView.onAdd: AddAnimation {
                id: animadd
                target: delegate
            }
            ListView.onRemove: RemoveAnimation {
                id: animremove
                target: delegate
            }

            Label {
                x: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                color: Theme.primaryColor
                textFormat: Text.StyledText
                text: "Title"
                width: parent.width - (2 * Theme.paddingLarge)
                elide: Text.ElideRight
                focus: false
            }
            onClicked: {
                console.log("Clicked " + name)
            }
        }
    }
}
