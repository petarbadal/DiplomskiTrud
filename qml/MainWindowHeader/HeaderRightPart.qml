import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import ".."
import "../CommonComponents"

Item {
    id: buttonsItem

    property var userReportDialogObject: 0
    property var assignProjectDialogObject: 0
    property var signUpDialogObject: 0

    function openUserReportDialog() {
        if(userReportDialogObject === 0) {
            userReportDialogObject = userReportDialogComponent.createObject(root)
            userReportDialogObject.open()
        } else
            userReportDialogObject.open()
    }

    function openAssignProjectDialog() {
        if(assignProjectDialogObject === 0) {
            assignProjectDialogObject = assignProjectDialogComponent.createObject(root)
            assignProjectDialogObject.open()
        } else
            assignProjectDialogObject.open()
    }

    function openSignUpDialog() {
        if(signUpDialogObject === 0) {
            signUpDialogObject = signUpDialogComponent.createObject(root)
            signUpDialogObject.open()
        } else
            signUpDialogObject.open()
    }

    Component.onCompleted: {
        if(timeStatusCore.isAdmin) {
            adminButtonsComp.createObject(buttonsItem)
        } else
            addTimeButtonComp.createObject(buttonsItem)
    }

    Label {
        id: labelHoursWeek

        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }
        visible: root.width > 1000
        text: "Hours this week: "
        font.pixelSize: 18
        font.bold: true
    }

    Label {
        id: labelHours

        anchors {
            left: labelHoursWeek.right
            verticalCenter: parent.verticalCenter
            leftMargin: 5
        }
        text: timeStatusCore.hoursSum
        font.pixelSize: 18
        font.bold: true
    }

    Image {
        id: logoutButton

        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 15
        }
        source: "qrc:/images/images/logout.svg"
        sourceSize.width: 25
        sourceSize.height: 30

        MouseArea {
            anchors.fill: parent

            onClicked: {
                logout()
            }
        }
    }

    Component {
        id: addTimeButtonComp

        CustomMediumButton {
            id: buttonAddTime

            anchors {
                right: logoutButton.left
                rightMargin: 15
                verticalCenter: parent.verticalCenter
            }
            height: 35
            width: 75
            buttonText: "Add Time"
            textPixelSize: 12
            mouseArea.onClicked: {
                root.openAddTimeDialog()
            }
        }
    }

    Component {
        id: adminButtonsComp

        RowLayout {
            anchors {
                verticalCenter: parent.verticalCenter
                right: logoutButton.left
                rightMargin: 15
            }
            spacing: 15
            CustomMediumButton {
                height: 35
                width: 75
                buttonText: "Assign"
                textPixelSize: 12
                mouseArea.onClicked: {
                    openAssignProjectDialog()
                }
            }

            CustomMediumButton {
                height: 35
                width: 75
                buttonText: "Get Report"
                textPixelSize: 12
                mouseArea.onClicked: {
                    openUserReportDialog()
                }
            }

            Image {
                id: signUpButton

                source: "qrc:/images/images/sign-up.svg"
                sourceSize.width: 25
                sourceSize.height: 30

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        openSignUpDialog()
                    }
                }
            }
        }
    }

    Component {
        id: userReportDialogComponent

        ManageUserReportsDialog {
            width: 700
            height: 650
            modal: true
            anchors.centerIn: parent
        }
    }

    Component {
        id: assignProjectDialogComponent

        AssignProjectDialog {
            width: 700
            height: 650
            modal: true
            anchors.centerIn: parent
        }
    }

    Component {
        id: signUpDialogComponent

        SignUpDialog {
            width: 340
            height: 340
            modal: true
            anchors.centerIn: parent
        }
    }
}
