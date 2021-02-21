import QtQuick 2.0
import QtQuick.Controls 1.4 as C14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4

Item {
    id: root

    property string titleText: ""

    property alias dateLabel: displayDate.text

    property bool calendarVisible: dateCalendar.visible

    property alias calendarComp: dateCalendar

    width: 200
    height: 55

    ColumnLayout {
        anchors.fill: parent
        spacing: 2

        Label {
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
            text: titleText
            font.family: "Lucida Console"
            font.pixelSize: 14
        }

        Rectangle {
            width: root.width
            height: 45
            color: "transparent"
            border.width: 1
            border.color: "black"
            radius: 10

            RowLayout {
                anchors {
                    fill: parent
                    leftMargin: 5
                }
                Image {
                    id: image

                    Layout.alignment: Qt.AlignLeft
                    source: "qrc:/images/images/calendarIcon.png"
                    sourceSize.width: 40
                    sourceSize.height: 40
                    visible: !dateCalendar.visible
                }

                Label {
                    id: displayDate

                    Layout.alignment: Qt.AlignLeft
                    visible: !dateCalendar.visible
                    text: ""
                    font.family: "Lucida Console"
                    font.pixelSize: 14
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent

        cursorShape: Qt.PointingHandCursor
        onClicked: {
            dateCalendar.visible = !dateCalendar.visible
        }
    }

    C14.Calendar {
        id: dateCalendar

        style: CalendarStyle {
            dayOfWeekDelegate: null
        }

        width: 230
        height: 200
        anchors.centerIn: parent

        frameVisible: false
        visible: false
        minimumDate: new Date(2010, 0, 1)
        maximumDate: new Date(2029, 11, 31)

        onClicked: {
            var splitDate = date.toLocaleDateString("en-US").split("/")
            var monthNo = splitDate[0].length === 1 ? String("0" + splitDate[0]) : splitDate[0]
            var dayNo = splitDate[1].length === 1 ? String("0" + splitDate[1]) : splitDate[1]
            var formatedDate = splitDate[2] + "-" + monthNo + "-" + dayNo
            displayDate.text = formatedDate
            visible = false
        }

        onSelectedDateChanged: {
//            var splitDate = selectedDate.toLocaleDateString("en-US").split("/")
//            var monthNo = splitDate[0].length === 1 ? String("0" + splitDate[0]) : splitDate[0]
//            var dayNo = splitDate[1].length === 1 ? String("0" + splitDate[1]) : splitDate[1]
//            var formatedDate = splitDate[2] + "-" + monthNo + "-" + dayNo
//            displayDate.text = formatedDate
//            visible = false
        }
    }
}
