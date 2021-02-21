import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

import "CommonComponents"
import BackendAccess 1.0

Popup {
    id: root

    property bool enabledSaveButton: (userComboBox.currentIndex > 1) && fileNameTF.text.length > 1

    function  getAllTableData() {
        var data = ""
        for(var i=0; i < reportsModel.count; i++) {
            var currentItem = reportsModel.get(i)
            var row = currentItem.date + ", " + currentItem.project + ", " + currentItem.description + ", " + currentItem.hours + "\n"
            data += row
        }

        return data
    }

    ListModel {
        id: projectsModel
    }

    ListModel {
        id: usersModel
    }

    ListModel {
        id: reportsModel
    }

    GetUserReport {
        id: getUserReportCore
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

        Connections {
            target: timeStatusCore

            onNewComboBoxUserReceived: {
                console.log("IN QML Manage User Report:", user)
                usersModel.append({username: user})
            }
            onClearAllUsersComboBoxData: usersModel.clear()
        }

        Connections {
            target: getUserReportCore

            onNewProjectReceived: {
                console.log("IN QML Manage User Report new Project:", projectName)
                projectsModel.append({project: projectName})
            }

            onClearProjects: projectsModel.clear()
            onClearReports: reportsModel.clear()
            //onClearUsers: usersModel.clear()
            onNewReportReceived: {
                //console.log("IN QML Manage User Report new Report detail:", reportDate, projectName, desc, spendTime)
                reportsModel.append({date: reportDate, project: projectName, description: desc, hours: spendTime})
            }
        }

        Label {
            id: title

            Layout.alignment: Qt.AlignLeft
            text: "Manage User Reports"
            font.family: "Lucida Console"
            font.pixelSize: 16
            font.bold: true
        }

        ColumnLayout {
            id: personRow

            anchors {
                top: title.bottom
                topMargin: 25
                left: parent.left
                leftMargin: 15
                right: parent.right
            }
            spacing: 15

            ColumnLayout {
                spacing: 2

                Label {
                    Layout.alignment: Qt.AlignLeft
                    text: "Choose User"
                    font.family: "Lucida Console"
                    font.pixelSize: 14
                    font.bold: true
                }

                CustomComboBox {
                    id: userComboBox

                    Layout.alignment: Qt.AlignLeft
                    model: usersModel

                    Component.onCompleted: {
                        timeStatusCore.getUsersForComboBox()
                        getUserReportCore.getUsersForComboBox()
                    }

                    onCurrentIndexChanged: {
                        if(currentIndex > -1) {
                            var currentUserValue = usersModel.get(currentIndex).username
                            if(currentUserValue !== "") {
                                getUserReportCore.setProjectValues(currentUserValue)
                                var currentProject = projectsModel.get(projectTumbler.currentIndex) ? projectsModel.get(projectTumbler.currentIndex).project : ""
                                var fromDate = fromDateCalendar.dateLabel
                                var toDate = toDateCalendar.dateLabel
                                if(currentUserValue && currentProject !== "" && fromDate !== "" && toDate !== "")
                                    getUserReportCore.getReportDetails(currentUserValue,
                                                                       currentProject,
                                                                       fromDate,
                                                                       toDate)
                            }
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true

                TextField {
                    id: fileNameTF

                    Layout.alignment: Qt.AlignTop
                    color: "black"
                    placeholderText: "Choose file name and path"
                    font.bold: true
                    font.pixelSize: 13
                    font.family: "Lucida Console"
                    selectByMouse: true

                    background: Rectangle {
                        implicitWidth: 350
                        implicitHeight: 25
                        color: "white"
                        border.color:  "transparent"
                        border.width: 1
                        radius: 3
                    }
                }

                Button {
                    height: 25
                    width: 30

                    contentItem: Text {
                        text: "File"
                        font.family: "Lucida Console"
                        font.bold: true
                        opacity: enabled ? 1.0 : 0.3
                        color: enabled ? "#23689b" : "gray"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    background: Rectangle {
                        implicitWidth: 30
                        implicitHeight: 25
                        color: "transparent"
                        border.color: enabled ? "#23689b" : "lightblue"
                        border.width: 4
                        radius: 5
                    }


                    onClicked: {
                        fileDialogComponent.createObject(root)
                    }
                }
            }

            Item {
                height: 60
                width: 20
            }
        }

        RowLayout {
            id: mainRow

            spacing: 40
            anchors {
                top: personRow.bottom
                topMargin: 15
                left: parent.left
                leftMargin: 15
                right: parent.right
                rightMargin: 15
            }

            CustomCalendar {
                id: fromDateCalendar

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
                titleText: "From Date"

                onDateLabelChanged: {
                    if(dateLabel !== "") {
                        var currentUserValue = usersModel.get(userComboBox.currentIndex).username
                        if(currentUserValue !== "") {
                            var currentProject = projectsModel.get(projectTumbler.currentIndex) ? projectsModel.get(projectTumbler.currentIndex).project : ""
                            var fromDate = fromDateCalendar.dateLabel
                            var toDate = toDateCalendar.dateLabel
                            if(currentUserValue && currentProject !== "" && fromDate !== "" && toDate !== "")
                                getUserReportCore.getReportDetails(currentUserValue,
                                                                   currentProject,
                                                                   fromDate,
                                                                   toDate)
                        }
                    }
                }
            }

            CustomCalendar {
                id: toDateCalendar

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
                titleText: "To Date"

                onDateLabelChanged: {
                    if(dateLabel !== "") {
                        var currentUserValue = usersModel.get(userComboBox.currentIndex).username
                        if(currentUserValue !== "") {
                            var currentProject = projectsModel.get(projectTumbler.currentIndex) ? projectsModel.get(projectTumbler.currentIndex).project : ""
                            var fromDate = fromDateCalendar.dateLabel
                            var toDate = toDateCalendar.dateLabel
                            if(currentUserValue && currentProject !== "" && fromDate !== "" && toDate !== "")
                                getUserReportCore.getReportDetails(currentUserValue,
                                                                   currentProject,
                                                                   fromDate,
                                                                   toDate)
                        }
                    }
                }
            }

            ColumnLayout {
                id: projectColumn

                spacing: 10
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "List of all Projects"
                    font.family: "Lucida Console"
                    font.pixelSize: 14
                    font.bold: true
                }

                CustomTumbler {
                    id: projectTumbler

                    Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
                    model: projectsModel
                    projectModel: true
                    currentIndex: 0

                    onCurrentIndexChanged: {
                        var currentUserValue = usersModel.get(userComboBox.currentIndex).username
                        if(currentUserValue !== "") {
                            var currentProject = projectsModel.get(currentIndex) ? projectsModel.get(currentIndex).project : ""
                            var fromDate = fromDateCalendar.dateLabel
                            var toDate = toDateCalendar.dateLabel
                            if(currentUserValue && currentProject !== "" && fromDate !== "" && toDate !== "")
                                getUserReportCore.getReportDetails(currentUserValue,
                                                                   currentProject,
                                                                   fromDate,
                                                                   toDate)
                        }
                    }
                }
            }
        }

        ColumnLayout {
            id: bottomRow

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

            CustomTableView {
                property int columnWidth: parent.width / 4

                height: 120
                Layout.fillWidth: true

                headerModel: [ // widths must add to 1
                    {text: "Date", keyword: "date", width: 0.25},
                    {text: "Project", keyword: "project", width: 0.25},
                    {text: "Work Description", keyword: "description", width: 0.25},
                    {text: "Hours", keyword: "hours", width: 0.25}
                ]

                dataModel: reportsModel
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

                    if(userReportDialogObject !== 0) {
                        userReportDialogObject.destroy()
                        userReportDialogObject = 0
                    }

                    timeStatusCore.onDataUpdated()
                }
            }
        }

        Button {
            Layout.alignment: Qt.AlignRight
            width: 100
            height: 30
            enabled: root.enabledSaveButton

            contentItem: Text {
                text: "SAVE REPORT"
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
                root.close()

                if(userReportDialogObject !== 0) {
                    userReportDialogObject.destroy()
                    userReportDialogObject = 0
                }

                getUserReportCore.onButtonSaveReportClicked(fileNameTF.text, getAllTableData())

                timeStatusCore.onDataUpdated()
            }
        }
    }

    Component {
        id: fileDialogComponent

        FileDialog {
            id: fileDialog

            title: "Save user report"
            folder: shortcuts.home
            nameFilters: ["CSV (Comma delimited) (*.csv)", "All files (*)"]
            selectExisting: false

            onAccepted: {
                console.log("You chose: " + fileDialog.fileUrls)
                var filePath = String(fileDialog.fileUrl).split("///")
                fileNameTF.text = filePath[1]
                fileDialog.close()
            }
            onRejected: {
                fileDialog.close()
            }
            Component.onCompleted: visible = true
        }
    }
}

