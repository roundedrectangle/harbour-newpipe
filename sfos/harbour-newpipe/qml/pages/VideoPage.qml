import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.newpipe.extractor 1.0
import "../components"

Page {
    id: root
    property string name
    property string thumbnail
    property string url
    property MediaInfo mediaInfo: MediaInfo { id: mediaInfo }
    property alias source: mediaInfo.content

    Component.onCompleted: {
        extractor.downloadExtract(mediaInfo, url);
        comments.model.loadComments(extractor, url);
    }

    SilicaListView {
        id: comments
        model: CommentModel {}
        anchors.fill: parent

        VerticalScrollDecorator {}

        onContentYChanged: {
            var pos = contentHeight + originY - height - contentY;
            if ((pos < height) && !comments.model.loading && comments.model.more && comments.model.nextPage) {
                comments.model.loadComments(extractor, url);
            }
        }

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
                source: root.source
                thumbnail: root.thumbnail
            }

            Label {
                id: videoTitle
                x: Theme.paddingLarge
                width: parent.width - (2 * Theme.paddingLarge)
                color: Theme.highlightColor
                wrapMode: Text.Wrap
                text: root.name
            }

            MediaDetails {
                mediaInfo: root.mediaInfo
            }
        }

        delegate: CommentItem {
            url: root.url
            uploaderAvatar: model.uploaderAvatar
            uploaderName: model.uploaderName
            commentText: model.commentText
            replyCount: model.replyCount
            page: model.page
        }
    }
}

