import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.newpipe.extractor 1.0
import "../components"

Page {
    id: root
    property string url
    property string uploaderAvatar
    property string uploaderName
    property string commentText
    property int replyCount
    property PageRef page

    Component.onCompleted: {
        comments.model.loadComments(extractor, url);
    }

    SilicaListView {
        id: comments
        model: CommentModel {
            nextPage: root.page
        }
        anchors.fill: parent

        VerticalScrollDecorator {}

        PushUpMenu {
            MenuItem {
                //% "Load more..."
                text: qsTrId("newpipe-repliespage_load-more")
                onDelayedClick: {
                    comments.model.loadComments(extractor, url);
                }
                enabled: comments.model.more
            }
            enabled: !comments.model.loading
            busy: comments.model.loading
        }

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
            url: root.url
            uploaderAvatar: model.uploaderAvatar
            uploaderName: model.uploaderName
            commentText: model.commentText
            replyCount: model.replyCount
            page: model.page
        }
    }
}
