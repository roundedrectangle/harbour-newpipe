import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.newpipe.extractor 1.0
import "../components"

Page {
    id: page
    property string name
    property string thumbnail
    property string source
    property string url
    property MediaInfo mediaInfo

    Connections {
        target: extractor
        onExtracted: {
            if (url === page.url) {
                mediaInfo = extractor.getMediaInfo(url);
                if (mediaInfo) {
                    source = mediaInfo.getContent();
                }
            }
        }
    }

    SilicaListView {
        model: commentModel
        anchors.fill: parent

        VerticalScrollDecorator {}

        header: Column {
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
                source: page.source
                thumbnail: page.thumbnail
            }

            Label {
                id: videoTitle
                x: Theme.paddingLarge
                width: parent.width - (2 * Theme.paddingLarge)
                height: Theme.itemSizeMedium
                color: Theme.highlightColor
                wrapMode: Text.Wrap
                text: page.name
            }
        }

        delegate: CommentItem {
            uploaderAvatar: model.uploaderAvatar
            uploaderName: model.uploaderName
            commentText: model.commentText
        }
    }
}

