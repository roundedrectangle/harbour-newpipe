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
    readonly property bool expandable: expandedHeight > collapsedHeight

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
        enabled: expandable
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
                    focus: false
                }
            }
        }
    }

    Icon {
        id: icon
        opacity: expandable ? 1.0 : 0.0
        Behavior on opacity { FadeAnimator {}}
        anchors {
            right: parent.right
            bottom: parent.bottom
            rightMargin: horizontalMargin
            bottomMargin: Theme.paddingSmall
        }
        source: "image://theme/icon-lock-more"
    }

    OpacityRampEffect {
        property real ratio: expandable ? (expandedHeight - height) / (expandedHeight - collapsedHeight)
                                        : 1.0
        slope: 2 * Math.min(1.0, ratio)
        offset: 0.5
        sourceItem: delegate
        enabled: expandable && !(open && !animation.running)
        direction: OpacityRamp.TopToBottom
    }
}
