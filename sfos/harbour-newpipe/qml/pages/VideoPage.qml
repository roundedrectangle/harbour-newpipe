import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.newpipe.extractor 1.0
import "../components"

Page {
    id: page
    property alias name: videoTitle.text
    property alias thumbnail: video.thumbnail
    property alias source: video.source
    property string url
    property MediaInfo mediaInfo

    Connections {
        target: extractor
        onExtracted: {
            if (url === page.url) {
                mediaInfo = extractor.getMediaInfo(url);
                if (mediaInfo) {
                    video.source = mediaInfo.getContent();
                }
            }
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        interactive: true
        contentHeight: column.height + Theme.paddingLarge


        VerticalScrollDecorator {}

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                id: header
                //% "YouTube Video"
                title: qsTrId("newpipe-video_page-header")
            }

            VideoPlayer {
                id: video
                width: parent.width
                height: width * (9 / 16)
            }

            Label {
                id: videoTitle
                x: Theme.paddingLarge
                width: parent.width - (2 * Theme.paddingLarge)
                height: Theme.itemSizeMedium
                color: Theme.highlightColor
                wrapMode: Text.Wrap
                text: name
            }
        }
    }
}
