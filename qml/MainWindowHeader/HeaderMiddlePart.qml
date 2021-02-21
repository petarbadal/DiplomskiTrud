import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {

    Button {
        id: leftButton

        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }

        contentItem: Rectangle {
            implicitWidth: 45
            implicitHeight: 40
            radius: 15
            border.color: "black"
            border.width: 1
            color: lbCursorMA.containsMouse ? "lightgray" : "transparent"

            Image {
                anchors.centerIn: parent
                source: "qrc:/images/images/lA.png"
                sourceSize.width: 35
                sourceSize.height: 30
            }
        }

        background: Rectangle {
            color: "transparent"
            implicitHeight: 30
            implicitWidth: 35
        }

        MouseArea {
            id: lbCursorMA

            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: leftButton.clicked()
        }

        onClicked: timeStatusCore.buttonPreviousWeekClicked()
    }

    Label {
        id: labelFirstDay

        anchors {
            right: labelMiddle.left
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }
        text: timeStatusCore.firstDay
        font.pixelSize: 18
        font.bold: true
    }

    Label {
        id: labelMiddle

        anchors.centerIn: parent
        text: "-"
        font.pixelSize: 18
        font.bold: true
    }

    Label {
        id:labelLastDay

        anchors {
            left: labelMiddle.right
            verticalCenter: parent.verticalCenter
            leftMargin: 10
        }
        text: timeStatusCore.lastDay
        font.pixelSize: 18
        font.bold: true
    }

    Label {
        anchors {
            top: labelMiddle.bottom
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
        text: timeStatusCore.year
        font.pixelSize: 18
        font.bold: true
    }

    Button {
        id: rightButton

        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
        }

        contentItem: Rectangle {
            implicitWidth: 45
            implicitHeight: 40
            radius: 15
            border.color: "black"
            border.width: 1
            color: rbCursorMA.containsMouse ? "lightgray" : "transparent"

            Image {
                anchors.centerIn: parent
                source: "qrc:/images/images/rA.png"
                sourceSize.width: 35
                sourceSize.height: 30
            }
        }

        background: Rectangle {
            color: "transparent"
            implicitHeight: 30
            implicitWidth: 35
        }

        MouseArea {
            id: rbCursorMA

            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: rightButton.clicked()
        }

        onClicked: timeStatusCore.buttonNextWeekClicked()
    }
}
