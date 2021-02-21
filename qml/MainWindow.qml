import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4 as C1
import Qt.labs.qmlmodels 1.0
import QtCharts 2.3

import BackendAccess 1.0
import "CommonComponents"
import "MainWindowHeader"
import ".."

Window {
    id: root

    visible: true
    width: 800
    height: 600
    visibility: "FullScreen"
    //    flags: Qt.FramelessWindowHint | Qt.Window
    title: qsTr("Main Window")
    minimumHeight: 700
    minimumWidth: 1000
    Component.onCompleted: timeStatusCore.setupGuiData()

    onVisibilityChanged: {
        console.log("Visibility changed")
        if(visibility === Qt.WindowMaximized) {
            console.log("Its maximized")
            visibility = "FullScreen"
        }
    }

    property var addTimeDialogObject: 0

    function openAddTimeDialog() {
        if(addTimeDialogObject === 0) {
            addTimeDialogObject = addTimeDialogComponent.createObject(root)
            addTimeDialogObject.open()
        } else
            addTimeDialogObject.open()
    }

    TimeStatusCore {
        id: timeStatusCore
    }

    Shortcut {
        sequence: "Esc"
        onActivated: root.visibility = "Windowed"
    }

    Rectangle {
        anchors.fill: parent
        color: "lightblue"
    }

    ColumnLayout {
        anchors {
            topMargin: 10
            fill: parent
            margins: 5
        }
        width: root.width
        height: root.height
        spacing: 20

        RowLayout {
            spacing: 5
            width: parent.width
            height: 70

            HeaderLeftPart {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignLeft
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            HeaderMiddlePart {
                height: parent.height
                width: 400
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            HeaderRightPart {
                Layout.alignment: Qt.AlignRight
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

        RowLayout {
            height: 350
            spacing: 10

            GridLayout {
                height: parent.height
                Layout.fillWidth: true
                columns: 1
                rows: 1

                ChartView {
                    id: pieChartView

                    property alias pieSeriesAlias: pieSeries
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    width: parent.width
                    height: parent.height
                    title: "Top Projects"
                    titleFont.bold: true
                    titleFont.family: "Century Gothic"
                    titleFont.pixelSize: 11
                    theme: ChartView.ChartThemeBlueIcy
                    antialiasing: false
                    legend.font.pixelSize: 14
                    legend.font.family: "Century Gothic"


                    Connections {
                        target: timeStatusCore

                        onNewPieValueReceived: pieSeries.append(projectInfo, hours)
                        onClearAllData: pieSeries.clear()
                    }

                    PieSeries {
                        id: pieSeries
                        size: 1
                    }
                }
            }

            GridLayout {
                height: parent.height
                Layout.fillWidth: true
                columns: 1
                rows: 1

                ChartView {
                    title: "Weekly Summary"
                    titleFont.bold: true
                    titleFont.family: "Century Gothic"
                    titleFont.pixelSize: 11
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    legend.alignment: Qt.AlignBottom
                    theme: ChartView.ChartThemeBlueIcy
                    legend.visible: false
                    antialiasing: false

                    Connections {
                        target: timeStatusCore

                        onNewBarValueReceived: mySeries.append("", [monday, tuesday, wednesday, thursday, friday, saturday, sunday])
                        onClearAllData: mySeries.clear()
                    }

                    BarSeries {
                        id: mySeries

                        axisX: BarCategoryAxis {
                            gridVisible: false
                            titleFont.bold: true
                            titleFont.family: "Century Gothic"
                            titleFont.pixelSize: 11
                            min: "MON"
                            max: "SUN"
                            categories: ["MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"]
                        }
                        axisY: ValueAxis {
                            titleFont.bold: true
                            titleFont.family: "Century Gothic"
                            titleFont.pixelSize: 11
                            tickAnchor: 0
                            tickInterval: 1
                            min: 0
                            max: 12
                            tickCount: 13
                            labelFormat: "%d"
                        }
                    }
                }
            }
        }

        Label {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            height: 25
            text: "Details"
            font.pixelSize: 20
            font.bold: true
        }

        CustomTableView {
            id: detailsTable

            property int columnWidth: parent.width / 5

            height: 120
            Layout.fillWidth: true

            headerModel: [ // widths must add to 1
                {text: "Project Name", keyword: "projectName", width: 0.111},
                {text: "Monday", keyword: "monday", width: 0.111},
                {text: "Tuesday", keyword: "tuesday", width: 0.111},
                {text: "Wednesday", keyword: "wednesday", width: 0.111},
                {text: "Thursday", keyword: "thursday", width: 0.111},
                {text: "Friday", keyword: "friday", width: 0.111},
                {text: "Saturday", keyword: "saturday", width: 0.111},
                {text: "Sunday", keyword: "sunday", width: 0.111},
                {text: "Total", keyword: "total", width: 0.111}
            ]

            dataModel: projectsController.projectsModel

//            onClicked: print('onClicked', row, JSON.stringify(rowData))
        }

        CustomTableView {
            id: descriptionTable

            property int columnWidth: parent.width / 5

            height: 120
            Layout.fillWidth: true

            headerModel: [ // widths must add to 1
                {text: "Date", keyword: "date", width: 0.2},
                {text: "Project", keyword: "project", width: 0.2},
                {text: "Work Description", keyword: "description", width: 0.2},
                {text: "Hours", keyword: "hours", width: 0.2},
                {text: "Options", keyword: "options", width: 0.2}
            ]

            dataModel: reportsController.reportsModel
        }
    }

    Connections {
        target: timeStatusCore

        onNewReportReceived: reportsController.createReportObjects(reportDate, projectName, desc, spendTime)
        onNewProjectReceived: projectsController.createProjectObjects(project, row)
        onNewProjectHoursReceived: projectsController.setProjectHours(date, value, j)
        onClearAllData: {
            projectsController.projectsModel.clear()
            reportsController.reportsModel.clear()
        }
    }

    ReportsController {
        id: reportsController
    }

    ProjectsController {
        id: projectsController
    }

    AddTimeCore {
        id: addTimeCore
    }

    Component {
        id: addTimeDialogComponent

        AddTimeDialog {
            id: addTimeDialog

            width: 560
            height: 600
            modal: true
            anchors.centerIn: parent
        }
    }
}
