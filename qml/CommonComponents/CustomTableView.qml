import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id: root

// public
    property variant headerModel: []

    property ListModel dataModel

    signal clicked(int row, variant rowData);

// private
    width: 500;  height: 200

    Rectangle {
        id: header

        width: parent.width
        height: 0.20 * root.height
        color: "lightgray"
        radius: 0.02 * root.width

        Rectangle { // half height to cover bottom rounded corners
            width: parent.width
            height: 0.5 * parent.height
            color: parent.color
            anchors.bottom: parent.bottom
        }

        ListView { // header
            anchors.fill: parent
            orientation: ListView.Horizontal
            interactive: false

            model: headerModel

            delegate: Item { // cell
                width: modelData.width * root.width
                height: header.height

                Text {
                    text: modelData.text
                    anchors.centerIn: parent
                    font.pixelSize: 13
                    font.family: "Century Gothic"
                    font.bold: true
                    color: 'white'
                }
            }
        }
    }

    ListView { // data
        anchors{
            fill: parent
            topMargin: header.height
        }
        interactive: true
        clip: true

        model: dataModel

        delegate: Item { // row
            width: root.width
            height: header.height * 0.9
            opacity: 1
            clip: true

            property int      row:     index     // outer index
            property QtObject rowData: model // much faster than listView.model[row]

            MouseArea {
                id: mouseArea

                hoverEnabled: true
                anchors.fill: parent
                propagateComposedEvents: true
            }

            Row {
                anchors.fill: parent

                Repeater { // index is column
                    model: headerModel
                    clip: true
                    delegate: Rectangle { // cell
                        width: headerModel[index].width * root.width
                        height: header.height * 0.9
                        color: mouseArea.containsMouse ? "lightgray" : "transparent"
                        clip: true
                        Text {
                            visible: modelData.keyword !== "options"
                            text: modelData.keyword !== "options" ? rowData[modelData.keyword] : ""
                            anchors.centerIn: parent
                            font.pixelSize: 12
                            font.family: "Century Gothic"
                            clip: true
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAnywhere
                        }

                        Rectangle {
                            id: buttonEdit

                            height: header.height * 0.8
                            width: header.height * 0.6
                            color: "transparent"
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            visible: modelData.keyword === "options"

                            Image {
                                anchors.centerIn: parent
                                source: "qrc:/images/images/editIcon.png"
                                sourceSize.height: header.height * 0.8
                                sourceSize.width: header.height * 0.6
                            }

                            MouseArea {
                                anchors.fill: parent

                                cursorShape: Qt.PointingHandCursor
                                hoverEnabled: true
                                onClicked: {
                                    console.log("Edit button clicked")

                                    var date = rowData.date
                                    var splitDate = date.split("-")
                                    var year = splitDate[0]
                                    var month = parseInt(splitDate[1]) - 1
                                    var day = parseInt(splitDate[2])


                                    console.log("Edit Clicked: ", year, month, day)
                                    console.log("Hours: ", rowData.hours)
                                    console.log("Project: ", rowData.project)

                                    openAddTimeDialog()
                                    addTimeDialogObject.currentHours = rowData.hours - 1
                                    addTimeDialogObject.currentDay = day
                                    addTimeDialogObject.currentMonth = month
                                    addTimeDialogObject.currentYear = year
                                    addTimeDialogObject.editProjectName = rowData.project
                                    addTimeDialogObject.editDescription = rowData.description
                                    addTimeDialogObject.editButtonClicked = true
                                }
                            }
                        }

                        Rectangle {
                            id: buttonDelete

                            height: header.height * 0.75
                            width: header.height * 0.5
                            color: "transparent"
                            anchors.right: parent.right
                            anchors.rightMargin: 40
                            visible: modelData.keyword === "options"

                            Image {
                                anchors.centerIn: parent
                                source: "qrc:/images/images/deleteIcon.png"
                                sourceSize.height: header.height * 0.75
                                sourceSize.width: header.height * 0.5
                            }

                            MouseArea {
                                anchors.fill: parent

                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor

                                onClicked: {
                                    console.log(rowData.project, rowData.date)
                                    timeStatusCore.onButtonDeleteClicked(rowData.project, rowData.date)
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                anchors {
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                }
                width: parent.width
                height: 1
                color: "white"
            }
        }
    }
}
