import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import harbour.newpipe.extractor 1.0

Page {
    id: page
    property alias name: videoTitle.text
    property alias thumbnail: image.source
    property string url
    property MediaInfo mediaInfo

    MediaPlayer {
        id: media
        autoPlay: true
    }

    Connections {
        target: extractor
        onExtracted: {
            if (url === page.url) {
                mediaInfo = extractor.getMediaInfo(url);
                if (mediaInfo) {
                    media.source = mediaInfo.getContent();
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

            VideoOutput {
                id: video
                width: parent.width
                height: width * (9 / 16)
                fillMode: Image.PreserveAspectFit
                source: media

                Image {
                    id: image
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    visible: media.source == ""
                }
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
