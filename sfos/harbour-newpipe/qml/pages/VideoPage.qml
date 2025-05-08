import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0

Page {
    property alias name: videoTitle.text
    property alias thumbnail: image.source
    property alias url: media.source

    MediaPlayer {
        id: media
        autoPlay: true
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
            }

            Image {
                id: image
                width: parent.width
                height: width * (9 / 16)
                fillMode: Image.PreserveAspectFit
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
