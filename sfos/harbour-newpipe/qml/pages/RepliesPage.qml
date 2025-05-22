import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.newpipe.extractor 1.0
import "../components"

Page {
    id: page
    property string url
    property string uploaderAvatar
    property string uploaderName
    property string commentText
    property int replyCount
    property PageRef page

//    Connections {
//        target: extractor
//        onExtracted: {
//            if (url === page.url) {
//                mediaInfo = extractor.getMediaInfo(url);
//                if (mediaInfo) {
//                    source = mediaInfo.getContent();
//                }
//            }
//        }
//    }

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
                //% "Replies"
                title: qsTrId("newpipe-replies_page-header")
            }
        }

        delegate: CommentItem {
            url: url
            uploaderAvatar: model.uploaderAvatar
            uploaderName: model.uploaderName
            commentText: model.commentText
            replyCount: model.replyCount
            page: model.page
        }
    }
}
