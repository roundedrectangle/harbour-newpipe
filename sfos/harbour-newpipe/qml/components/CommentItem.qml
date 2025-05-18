import QtQuick 2.0
import Sailfish.Silica 1.0

SilicaControl {
    id: root

    property bool open: false
    property bool _open: false
    property int horizontalMargin: Theme.horizontalPageMargin
    property int bottomMargin: Theme.paddingMedium
    property int collapsedHeight: Theme.itemSizeExtraLarge
    property int expandedHeight: Theme.fontSizeExtraSmall + Theme.paddingSmall + mainCommentText.contentHeight + (2 * Theme.paddingSmall)

    property string uploaderAvatar
    property string uploaderName
    property string commentText

    signal clicked

    width: parent ? parent.width : Screen.width
    height: _open ? expandedHeight : collapsedHeight

    highlighted: content.pressed && content.containsMouse

    onClicked: open = !open
    onOpenChanged: {
        heightBehavior.enabled = true
        _open = open
        heightBehavior.enabled = false
    }

    Behavior on height {
        id: heightBehavior
        enabled: false
        NumberAnimation {
            id: animation
            target: root
            property: "height"
            duration: expandedHeight > Screen.height ? 400 : 200
            easing.type: Easing.InOutQuad
        }
    }

    MouseArea {
        id: content
        anchors {
            fill: parent
            bottomMargin: root.bottomMargin
        }
        onClicked: parent.clicked()
    }

    Item {
        id: delegate
        focus: false
        anchors.fill: parent

        Row {
            id: commentRow
            x: Theme.paddingLarge
            y: Theme.paddingSmall
            width: parent.width - (2 * Theme.paddingLarge)
            height: parent.height - (2 * Theme.paddingSmall)
            spacing: Theme.paddingLarge

            Image {
                id: avatar
                y: Theme.paddingSmall
                width: Theme.iconSizeMedium
                height: Theme.iconSizeMedium
                fillMode: Image.PreserveAspectFit
                verticalAlignment: Image.AlignTop
                source: uploaderAvatar
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
                    text: uploaderName
                    elide: Text.ElideRight
                    focus: false
                }

                Label {
                    id: mainCommentText
                    color: Theme.primaryColor
                    textFormat: Text.StyledText
                    width: parent.width
                    height: commentRow.height - Theme.fontSizeExtraSmall - Theme.paddingSmall
                    font.pixelSize: Theme.fontSizeSmall
                    text: commentText
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    maximumLineCount: _open ? (2^31 - 1) : 3
                    focus: false
                }
            }
        }
    }
}
