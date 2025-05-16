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

        delegate: BackgroundItem {
            id: delegate
            focus: false
            height: Theme.itemSizeExtraLarge

            Row {
                id: commentRow
                x: Theme.paddingLarge
                y: Theme.paddingSmall
                width: parent.width - (2 * Theme.paddingLarge)
                height: parent.height - (2 * Theme.paddingSmall)
                spacing: Theme.paddingLarge

                Image {
                    id: avatar
                    width: Theme.iconSizeMedium
                    height: parent.height
                    fillMode: Image.PreserveAspectFit
                    verticalAlignment: Image.AlignTop
                    source: model.uploaderAvatar
                }

                Column {
                    id: comment
                    width: parent.width - avatar.width - Theme.paddingLarge
                    spacing: Theme.paddingSmall

                    Label {
                        color: Theme.primaryColor
                        textFormat: Text.PlainText
                        width: parent.width
                        height: Theme.fontSizeExtraSmall
                        font.pixelSize: Theme.fontSizeExtraSmall
                        text: model.uploaderName
                        elide: Text.ElideRight
                        focus: false
                    }

                    Label {
                        color: Theme.primaryColor
                        textFormat: Text.StyledText
                        width: parent.width
                        height: commentRow.height - Theme.fontSizeExtraSmall - Theme.paddingSmall
                        font.pixelSize: Theme.fontSizeSmall
                        text: model.commentText
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        truncationMode: TruncationMode.Elide
                        maximumLineCount: 2
                        focus: false
                    }
                }
            }
        }
    }
}

