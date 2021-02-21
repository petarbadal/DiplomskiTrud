import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


import "CommonComponents"

import BackendAccess 1.0

Popup {
    id: root

    property int charactersLeft: 60

    property bool enableAssignButton: (projectNameTF.text.length > 2) && (descriptionTA.text.length >= 60)

    Component.onCompleted: {
        console.log("QML CALL SET USERS COMBOBOX")
        timeStatusCore.getUsersForComboBox()
    }

    ListModel {
        id: usersModel
    }

    Connections {
        target: timeStatusCore

        onNewComboBoxUserReceived: {
            console.log("IN QML MAIN WINDOW:", user)
            usersModel.append({username: user})
        }
        onClearAllUsersComboBoxData: usersModel.clear()
    }

    AssignProjectCore {
        id: assignProjectCore
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

        Label {
            id: windowTitle

            anchors {
                top: parent.top
                topMargin: 10
                left: parent.left
                leftMargin: 15
            }
            text: "Fill Project Details"
            font.family: "Lucida Console"
            font.pixelSize: 15
            font.bold: true
        }

        ColumnLayout {
            id: topRow

            spacing: 5
            Layout.fillWidth: true
            anchors {
                top: windowTitle.bottom
                topMargin: 10
                left: parent.left
                leftMargin: 15
                right: parent.right
                rightMargin: 15
            }

            TextField {
                id: projectNameTF

                Layout.alignment: Qt.AlignTop
                color: "black"
                placeholderText: "Project Name"
                font.bold: true
                font.pixelSize: 13
                font.family: "Lucida Console"

                background: Rectangle {
                    implicitWidth: 350
                    implicitHeight: 25
                    color: "white"
                    border.color:  "transparent"
                    border.width: 1
                    radius: 3
                }
            }

            TextField {
                id: companyNameTF

                Layout.alignment: Qt.AlignTop
                color: "black"
                placeholderText: "Company Name"
                font.bold: true
                font.pixelSize: 13
                font.family: "Lucida Console"

                background: Rectangle {
                    implicitWidth: 350
                    implicitHeight: 25
                    color: "white"
                    border.color:  "transparent"
                    border.width: 1
                    radius: 3
                }
            }

            TextField {
                id: clientNameTF

                Layout.alignment: Qt.AlignTop
                color: "black"
                placeholderText: "Client Name"
                font.bold: true
                font.pixelSize: 13
                font.family: "Lucida Console"

                background: Rectangle {
                    implicitWidth: 350
                    implicitHeight: 25
                    color: "white"
                    border.color:  "transparent"
                    border.width: 1
                    radius: 3
                }
            }
            Item {
                height: 40
                width: 20
            }
        }

        RowLayout {
            id: mainRow

            spacing: 48
            anchors {
                top: topRow.bottom
                topMargin: 10
                left: parent.left
                leftMargin: 15
                right: parent.right
                rightMargin: 15
            }

            CustomCalendar {
                id: fromDateCal

                Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
                titleText: "From Date"
            }

            CustomCalendar {
                id: toDateCal

                Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
                titleText: "To Date"
            }

            ColumnLayout {
                id: projectColumn

                spacing: 10
                Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Assign To User"
                    font.family: "Lucida Console"
                    font.pixelSize: 14
                    font.bold: true
                }

                CustomTumbler {
                    id: userTumbler

                    Layout.alignment: Qt.AlignHCenter
                    model: usersModel
                    projectModel: false
                    currentIndex: 0
                }
            }
        }

        ColumnLayout {
            id: descriptionLayout

            clip: true
            anchors {
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

                    if(assignProjectDialogObject !== 0) {
                        assignProjectDialogObject.destroy()
                        assignProjectDialogObject = 0
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
            enabled: enableAssignButton

            contentItem: Text {
                text: "ASSIGN"
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
                if(assignProjectCore.buttonAssignProjectClicked(projectNameTF.text,
                                                                descriptionTA.text,
                                                                clientNameTF.text,
                                                                companyNameTF.text,
                                                                fromDateCal.dateLabel,
                                                                toDateCal.dateLabel,
                                                                usersModel.get(userTumbler.currentIndex).username)) {
                    root.close()

                    if(assignProjectDialogObject !== 0) {
                        assignProjectDialogObject.destroy()
                        assignProjectDialogObject = 0
                    }

                    timeStatusCore.onDataUpdated()
                }
            }
        }
    }
}
