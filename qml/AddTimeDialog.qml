import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "CommonComponents"

Popup {
    id: root

    property int charactersLeft: 60
    property int currentHours: 0
    property int currentDay: 1
    property int currentMonth: 0
    property int currentYear: 2010

    property alias projectTumbler: projectTumbler

    property bool editButtonClicked: false

    property string editProjectName: ""
    property string editDescription: ""

    onEditButtonClickedChanged: {
        if(editButtonClicked) {
            dateCal.calendarComp.clicked( new Date(currentYear, currentMonth, currentDay))
            dateCal.enabled = false
            hoursTumbler.currentIndex = currentHours
            descriptionTA.text = editDescription
            projectTextField.placeholderText = editProjectName
        }
    }

    Component.onCompleted: {
        addTimeCore.setProjectValues(timeStatusCore.currentUser)
    }

    Connections {
        target: addTimeCore

        onNewProjectReceived: {
            console.log("IN QML AddTime new Project:", projectName)
            projectsModel.append({project: projectName})
        }

        onClearProjects: projectsModel.clear()
    }

    ListModel {
        id: projectsModel
    }

    Frame {
        id: mainContentFrame

        clip: true
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: parent.height - 70

        background: Rectangle {
            color: "lightblue"
        }

        ColumnLayout {
            id: personRow

            anchors {
                top: parent.top
                topMargin: 10
                left: parent.left
                leftMargin: 15
                right: parent.right
            }
            spacing: 2

            Label {
                Layout.alignment: Qt.AlignLeft
                text: "Person"
                font.family: "Lucida Console"
                font.pixelSize: 14
                font.bold: true
            }

            TextField {
                id: personTextField

                Layout.alignment: Qt.AlignLeft
                color: "black"
                readOnly: true
                placeholderText: timeStatusCore.currentUser
                font.bold: true
                font.pixelSize: 13
                font.family: "Lucida Console"

                background: Rectangle {
                    implicitWidth: 280
                    implicitHeight: 20
                    color: "white"
                    border.color:  "transparent"
                    border.width: 1
                    radius: 3
                }
            }

            Item {
                height: 20
                width: 20
            }
        }

        RowLayout {
            id: mainRow

            spacing: 48
            anchors {
                top: personRow.bottom
                topMargin: 15
                left: parent.left
                leftMargin: 15
                right: parent.right
                rightMargin: 15
            }

            ColumnLayout {
                id: dateColumn

                spacing: 15
                Layout.alignment: Qt.AlignLeft

                CustomCalendar {
                    id: dateCal

                    Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
                    titleText: "Date"
                }
            }

            ColumnLayout {
                id: spendTimeRow

                spacing: 15
                Layout.alignment: Qt.AlignLeft

                Label {
                    height: 30
                    width: parent.width
                    text: "Time Spend"
                    font.family: "Lucida Console"
                    font.pixelSize: 14
                    font.bold: true
                }

                ColumnLayout {
                    spacing: 2

                    Label {
                        text: "Hours"
                        font.family: "Lucida Console"
                        font.pixelSize: 11
                    }

                    CustomTumbler {
                        id: hoursTumbler

                        model: 24
                        currentIndex: 2
                        integerModel: true
                    }
                }
            }

            ColumnLayout {
                id: projectColumn

                spacing: 15
                Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter

                Label {
                    text: "Project Name"
                    font.family: "Lucida Console"
                    font.pixelSize: 14
                    font.bold: true
                }

                CustomTumbler {
                    id: projectTumbler

                    visible: !editButtonClicked
                    Layout.alignment: Qt.AlignHCenter
                    model: projectsModel
                    projectModel: true
                    currentIndex: 0
                }

                TextField {
                    id: projectTextField

                    visible: editButtonClicked
                    Layout.alignment: Qt.AlignHCenter
                    color: "black"
                    readOnly: true
                    placeholderText: ""
                    font.bold: true
                    font.pixelSize: 10
                    font.family: "Lucida Console"

                    background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 20
                        color: "white"
                        border.color:  "transparent"
                        border.width: 1
                        radius: 3
                    }
                }
            }
        }

        ColumnLayout {
            id: descriptionLayout

            clip: true
            anchors {
                top: mainRow.bottom
                left: parent.left
                leftMargin: 15
                right: parent.right
                rightMargin: 15
                bottom: parent.bottom
                bottomMargin: 10
            }

            Item {
                height: 40
                width: 20
            }

            TextArea {
                id: descriptionTA

                color: "gray"
                placeholderText: "Enter description (Minimum 60 symbols)"
                font.bold: true
                font.pixelSize: 11
                font.family: "Lucida Console"
                clip: true
                wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
                width: mainContentFrame.width - 70
                height: 140
                selectByMouse: true

                background: Rectangle {
                    implicitWidth: mainContentFrame.width - 50
                    implicitHeight: 140
                    color: "white"
                    border.color: parent.activeFocus ? "lightblue" : "transparent"
                    clip: true
                }

                onTextChanged: {
                    if ((60 - text.length) < 0)
                        charactersLeft = 0
                    else
                        charactersLeft = 60 - text.length
                }
            }
        }
    }

    RowLayout {
        id: buttonsRow

        anchors {
            top: mainContentFrame.bottom
            topMargin: 10
            bottom: parent.bottom
        }
        implicitHeight: 50
        width: parent.width

        RowLayout {
            spacing: 5

            Button {
                Layout.alignment: Qt.AlignLeft
                width: 100
                height: 30

                contentItem: Text {
                    text: "CANCEL"
                    font.family: "Lucida Console"
                    font.bold: true
                    opacity: enabled ? 1.0 : 0.3
                    color: "#23689b"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 40
                    color: "transparent"
                    border.color: "#23689b"
                    border.width: 4
                    radius: 5
                }

                onClicked: {
                    root.close()

                    if(addTimeDialogObject !== 0) {
                        addTimeDialogObject.destroy()
                        addTimeDialogObject = 0
                    }
                }
            }

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "Remaining: " + charactersLeft
                color: "lightgray"
            }
        }

        Button {
            Layout.alignment: Qt.AlignRight
            width: 100
            height: 30
            enabled: (descriptionTA.text.length >= 60)

            contentItem: Text {
                text: editButtonClicked ? "EDIT" : "ADD TIME"
                font.family: "Lucida Console"
                font.bold: true
                opacity: enabled ? 1.0 : 0.3
                color: enabled ? "#23689b" : "gray"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                color: "transparent"
                border.color: enabled ? "#23689b" : "lightblue"
                border.width: 4
                radius: 5
            }

            onClicked: {
                console.log("IN QML:", timeStatusCore.currentUser,
                            dateCal.dateLabel,
                            hoursTumbler.currentIndex + 1,
                            editProjectName,
                            descriptionTA.text,
                            editButtonClicked)

                var project = editButtonClicked ? editProjectName : projectsModel.get(projectTumbler.currentIndex).project
                if(addTimeCore.buttonAddTimeClicked(timeStatusCore.currentUser,
                                                    dateCal.dateLabel,
                                                    hoursTumbler.currentIndex + 1,
                                                    project,
                                                    descriptionTA.text,
                                                    editButtonClicked)) {
                    timeStatusCore.onDataUpdated()

                    if(addTimeDialogObject !== 0) {
                        addTimeDialogObject.destroy()
                        addTimeDialogObject = 0
                    }
                }
                root.close()
            }
        }
    }
}
