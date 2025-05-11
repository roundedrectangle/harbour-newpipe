import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import harbour.newpipe.extractor 1.0

Item {
    property alias source: media.source
    property alias thumbnail: image.source
    property bool controlsvisible: false
    property int skipTimeShort: 10
    property int skipTimeLong: 60

    readonly property int controlgap: 2 * Theme.paddingLarge
    readonly property bool playing: (media.playbackState == MediaPlayer.PlayingState)
    property bool controllable: isControllable()
    property bool forceVisible: false

    width: parent.width
    height: width * (9 / 16)

    function isControllable() {
        var result = true;
        switch (media.status) {
        case MediaPlayer.NoMedia:
        case MediaPlayer.Loading:
        case MediaPlayer.InvalidMedia:
        case MediaPlayer.UnknownStatus:
            result = false;
        }
        return result;
    }

    onControllableChanged: {
        if (controllable == true) {
            forceVisible = true;
            openControls();
        }
    }

    function toggleControls() {
        if ((!controlsvisible) || (fadeout.running)) {
            console.log("Display controls")
            fadeout.stop()
            controls.opacity = 1
            controlsvisible = true
            controlsTimer.restart()
        }
        else {
            console.log("Remove controls")
            controlsTimer.stop()
            fadeout.stop()
            controlsvisible = false
        }
    }

    function openControls () {
        console.log("Display controls")
        fadeout.stop()
        controls.opacity = 1
        controlsvisible = true
        if (forceVisible) {
            controlsTimer.stop()
        }
        else {
            controlsTimer.restart()
        }
    }

    MediaPlayer {
        id: media
        autoPlay: false
        autoLoad: true
        onPositionChanged: {
            mediaslider.value = position
        }
        onAvailabilityChanged: console.log("Availability: " + availability)
        onSourceChanged: console.log("Source: " + source)
    }

    VideoOutput {
        id: video
        anchors.fill: parent
        width: parent.width
        height: width * (9 / 16)
        fillMode: Image.PreserveAspectFit
        source: media

        Image {
            id: image
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            visible: !controllable || forceVisible
        }
    }

    Timer {
        id: controlsTimer
        interval: 10000
        running: false
        repeat: false
        triggeredOnStart: false
        onTriggered: {
            console.log("Control fadeout")
            fadeout.start()
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("Video clicked")
            toggleControls()
        }
    }

    Item {
        id: controls
        anchors.fill: parent
        visible: (controllable && controlsvisible) || forceVisible
        opacity: 1

        NumberAnimation on opacity {
            id: fadeout
            from: 1
            to: 0
            duration: 1000
            onRunningChanged: {
                if (!running) {
                    console.log("Faded out, making invisible")
                    controlsvisible = false
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("Rectangle clicked")
                toggleControls()
            }
        }

        IconButton {
            id: playbutton
            width: Theme.iconSizeLarge
            height: Theme.iconSizeLarge
            icon.sourceSize.width: width
            icon.sourceSize.height: height
            icon.fillMode: Image.PreserveAspectFit
            anchors.verticalCenter: parent.verticalCenter
            anchors.bottomMargin: controlgap
            anchors.horizontalCenter: parent.horizontalCenter
            icon.source: (playing ? Qt.resolvedUrl("image://theme/icon-l-pause?") : Qt.resolvedUrl("image://theme/icon-l-play?"))
                    + (pressed ? Theme.highlightColor : Theme.primaryColor)

            onClicked: {
                if (forceVisible) {
                    forceVisible = false;
                    toggleControls();
                }
                else {
                    openControls()
                }
                console.log("MouseArea click 2")
                if (playing) {
                    console.log("Pause 2")
                    media.pause()
                }
                else {
                    console.log("Play 2")
                    media.play()
                }
            }
        }

        IconButtonDual {
            id: reversebutton
            width: Theme.iconSizeLarge
            height: Theme.iconSizeLarge
            anchors.verticalCenter: playbutton.verticalCenter
            anchors.right: playbutton.left
            anchors.rightMargin: controlgap
            icon.source: Qt.resolvedUrl("image://newpipe/icon-l-replay?") + (pressed ? Theme.highlightColor : Theme.primaryColor)

            onShortClick: {
                openControls()
                media.seek(media.position - (1000 * skipTimeShort))
                mediaslider.value = mediaslider.value - (1000 * skipTimeShort)
            }
            onLongClick: {
                openControls()
                media.seek(media.position - (1000 * skipTimeLong))
                mediaslider.value = mediaslider.value - (1000 * skipTimeLong)
            }
        }

        IconButtonDual {
            id: forwardsbutton
            width: Theme.iconSizeLarge
            height: Theme.iconSizeLarge
            anchors.verticalCenter: playbutton.verticalCenter
            anchors.left: playbutton.right
            anchors.leftMargin: controlgap
            icon.source: Qt.resolvedUrl("image://newpipe/icon-l-skip?") + (pressed ? Theme.highlightColor : Theme.primaryColor)

            onShortClick: {
                openControls()
                media.seek(media.position + (1000 * skipTimeShort))
                mediaslider.value = mediaslider.value + (1000 * skipTimeShort)
            }
            onLongClick: {
                openControls()
                media.seek(media.position + (1000 * skipTimeLong))
                mediaslider.value = mediaslider.value + (1000 * skipTimeLong)
            }
        }

        Slider {
            id: mediaslider
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            minimumValue: 0
            maximumValue: media.duration
            stepSize: 1
            value: 0
            enabled: media.seekable
            valueText: Utils.millisecondsToTime(sliderValue)
            onPressed: {
                openControls()
                controlsTimer.stop()
            }
            onReleased: {
                media.seek(sliderValue)
                openControls()
            }
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: true
        visible: !controllable
        size: BusyIndicatorSize.Medium
    }
}
