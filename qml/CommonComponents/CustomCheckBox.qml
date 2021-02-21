import QtQuick 2.0
import QtQuick.Controls 2.12

CheckBox {
    id: checkBox

    property string checkBoxText

    height: parent.height

    indicator: Rectangle {
        id: indicator

        x: checkBox.leftPadding
        y: parent.height / 2 - height / 2
        width: 15
        height: 15
        color: "white"
        border.width: 1
        border.color: checkBox.checked ? "dimgray" : "silver"
        radius: 3

        Rectangle {
            anchors.centerIn: parent
            width: 9
            height: 9
            color: "dimgray"
            visible: checkBox.checked
        }
    }

    contentItem: Text {
        anchors {
            left: indicator.right
            leftMargin: 5
        }
        text: checkBoxText
        font.family: "MS Shell Dlg 2"
        opacity: 0.8
        color: "black"
        verticalAlignment: Text.AlignVCenter
    }
}
