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
    property LinkHandlerModel linkHandlerModel: LinkHandlerModel { }

    Component.onCompleted: {
        extractor.getChannelInfo(channelInfo, linkHandlerModel, url);
    }

    Connections {
        target: extractor
        onExtracted: {
            var length = linkHandlerModel.count();
            console.log("Number of tabs: " + length);

            for (var pos = 0; pos < length; pos++) {
                var tab = linkHandlerModel.getLinkHandler(pos);
                console.log("Tab: " + tab.contentFilters()[0]);
            }
            var linkHandler = linkHandlerModel.getLinkHandler(0);
            console.log("Extracted URL: " + url);
            extractor.getChannelTabInfo(channelTabInfo, linkHandler)
        }
    }

    ViewPlaceholder {
        enabled: true
        textFormat: Text.RichText
        text: "Channel Page"
        hintText: "Under Construction"
    }
}

