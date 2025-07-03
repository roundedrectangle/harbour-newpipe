import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.newpipe.extractor 1.0
import "../components"

Page {
    id: root
    property string name
    property string thumbnail
    property string url
    property ChannelInfo channelInfo: ChannelInfo { }
    property ChannelTabInfo channelTabInfo: ChannelTabInfo { }

    Component.onCompleted: {
        extractor.getChannelInfo(channelInfo, url);
    }

    Connections {
        target: extractor
        onExtracted: {
            console.log("Extracted URL: " + url);
        }
    }

    ViewPlaceholder {
        enabled: true
        textFormat: Text.RichText
        text: "Channel Page"
        hintText: "Under Construction"
    }
}

