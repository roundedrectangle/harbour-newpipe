import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Silica.private 1.0
import harbour.newpipe.extractor 1.0
import "../components"

Page {
    id: root
    property string name
    property string thumbnail
    property string url
    property ChannelInfo channelInfo: ChannelInfo { }
    property ChannelTabInfo channelTabInfo: ChannelTabInfo { }
    property LinkHandlerModel linkHandlerModel: LinkHandlerModel { }

    Component.onCompleted: {
        extractor.getChannelInfo(channelInfo, linkHandlerModel, url);
    }

    Connections {
        target: extractor
        onExtracted: {
            var length = linkHandlerModel.count();
            console.log("Number of tabs: " + length);

            for (var pos = 0; pos < length; pos++) {
                var tab = linkHandlerModel.getLinkHandler(pos);
                console.log("Tab: " + tab.contentFilters()[0]);
            }
            var linkHandler = linkHandlerModel.getLinkHandler(0);
            console.log("Extracted URL: " + url);
            extractor.getChannelTabInfo(channelTabInfo, linkHandler)
        }
    }

    TabView {
        id: tabs

        anchors.fill: parent
        currentIndex: 1

        header: TabBar {
            model: tabBarModel
        }

        model: [aboutView, videoView, shortsView, liveView, playlistsView]

        Component {
            id: aboutView
            TabItem {
                anchors.fill: parent
                flickable: flickable

                SilicaFlickable {
                    id: flickable
                    anchors.fill: parent

                    ViewPlaceholder {
                        enabled: true
                        textFormat: Text.RichText
                        text: "About"
                        hintText: "Under Construction"
                    }
                }
            }
        }
        Component {
            id: videoView
            TabItem {
                anchors.fill: parent
                flickable: flickable

                SilicaFlickable {
                    id: flickable
                    anchors.fill: parent

                    ViewPlaceholder {
                        enabled: true
                        textFormat: Text.RichText
                        text: "Videos"
                        hintText: "Under Construction"
                    }
                }
            }
        }
        Component {
            id: shortsView
            TabItem {
                anchors.fill: parent
                flickable: flickable

                SilicaFlickable {
                    id: flickable
                    anchors.fill: parent

                    ViewPlaceholder {
                        enabled: true
                        textFormat: Text.RichText
                        text: "Shorts"
                        hintText: "Under Construction"
                    }
                }
            }
        }
        Component {
            id: liveView
            TabItem {
                anchors.fill: parent
                flickable: flickable

                SilicaFlickable {
                    id: flickable
                    anchors.fill: parent

                    ViewPlaceholder {
                        enabled: true
                        textFormat: Text.RichText
                        text: "Live feeds"
                        hintText: "Under Construction"
                    }
                }
            }
        }
        Component {
            id: playlistsView
            TabItem {
                anchors.fill: parent
                flickable: flickable

                SilicaFlickable {
                    id: flickable
                    anchors.fill: parent

                    ViewPlaceholder {
                        enabled: true
                        textFormat: Text.RichText
                        text: "Playlists"
                        hintText: "Under Construction"
                    }
                }
            }
        }
    }

    ListModel {
        id: tabBarModel

        ListElement {
            title: "About"
        }
        ListElement {
            title: "Videos"
        }
        ListElement {
            title: "Shorts"
        }
        ListElement {
            title: "Live"
        }
        ListElement {
            title: "Playlists"
        }
    }
}

