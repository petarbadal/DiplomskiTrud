import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

RowLayout {
    property string imageSource: ""
    property string labelText: ""
    property string usage: ""
    property string inputText: ""
    property string defaultPlaceholderText: ""

    property bool passwordMode: false

    property alias textField: textField

    signal enterKeyPressed

    height: 35

    Rectangle {
        Layout.fillWidth: true
        Layout.leftMargin: 10
        Layout.rightMargin: 10
        height: 35
        color: "lightgray"
        border.width: 2
        border.color: "gray"
        radius: 5

        Rectangle {
            id: userIcon

            height: 35
            width: imageSource !== "" ? 34 : 70
            color: "lightgray"
            border.width: 2
            border.color: "gray"
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }

            Component.onCompleted: {
                if(imageSource !== "") {
                    imageComp.createObject(userIcon)
                } else {
                    labelComp.createObject(userIcon)
                }
            }

            Component {
                id: imageComp

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    sourceSize {
                        width: 30
                        height: 30
                    }
                    clip: true
                    fillMode: Image.PreserveAspectFit
                    source: imageSource
                }
            }

            Component {
                id: labelComp

                Label {
                   anchors.left: parent.left
                   anchors.leftMargin: 5
                   anchors.verticalCenter: parent.verticalCenter
                   horizontalAlignment: Qt.AlignLeft
                   verticalAlignment: Qt.AlignVCenter
                   text: labelText
                }
            }
        }

        Rectangle {
            anchors {
                left: userIcon.right
                right: parent.right
                rightMargin: 3
                verticalCenter: parent.verticalCenter
            }
            height: parent.height - 4
            color: "white"

            TextField {
                id: textField

                anchors {
                    fill: parent
                    leftMargin: 5
                }
                color: "#393e39"
                font.family: "MS Shell Dlg 2"
                selectionColor: "#cbe368"
                selectedTextColor: "#ffffff"
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                font.pixelSize: 15
                clip: true
                maximumLength: 30
                echoMode: passwordMode ? TextInput.Password : TextInput.Normal
                text: inputText
                placeholderText: defaultPlaceholderText
                selectByMouse: true
                focus: true

                background: Rectangle {
                    color: "white"
                }

                onTextChanged: {
                    if(usage === "userName")
                        logInUserName = text
                    else if(usage === "userPassword")
                        logInUserPassword = text
                }

                onAccepted:  {
                    enterKeyPressed()
                }
            }
        }
    }
}
