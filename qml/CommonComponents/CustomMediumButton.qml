import QtQuick 2.0

Rectangle {
    property string buttonText
    property alias mouseArea: buttonMouseArea
    property int textPixelSize: 15

    color: "#cbe368"
    Text {
        anchors.centerIn: parent
        text: buttonText
        font.family: "Century Gothic"
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: textPixelSize
    }

    MouseArea {
        id: buttonMouseArea

        anchors.fill: parent
    }
}
