import QtQuick 2.12
import QtQuick.Controls 2.12

import "../CommonComponents"

Item {
    id: root

    Component.onCompleted: {
        console.log("IN HEADER LEFT PART, ", timeStatusCore.isAdmin)
        if(timeStatusCore.isAdmin)
            usersComboBoxComp.createObject(root)
        else
            timeStatusLabelComp.createObject(root)
    }

    Label {
        id: labelUserName

        anchors {
            left: parent.left
            top: parent.top
        }
        text: (timeStatusCore.isAdmin ? "Preview as user: " : "User: ") + timeStatusCore.currentUser
        font.pixelSize: 18
        font.bold: true

        Component.onCompleted: {
            console.log("IN HEADER LEFT PART, ", timeStatusCore.isAdmin)
        }
    }

    Component {
        id: usersComboBoxComp

        Item {
            anchors {
                left: parent.left
                bottom: parent.bottom
            }

            height: 30
            width: 150

            Connections {
                target: timeStatusCore

                onNewComboBoxUserReceived: {
                    console.log("IN QML MAIN WINDOW:", user)
                    usersModel.append({username: user})
                }
                onClearAllUsersComboBoxData: usersModel.clear()
            }

            Label {
                id: chooseUserLabel

                anchors.left: parent.left
                text: "Choose User: "
                font.pixelSize: 18
                font.bold: true
            }

            CustomComboBox {
                anchors {
                    left: chooseUserLabel.right
                    leftMargin: 5
                }
                model: usersModel

                onCurrentIndexChanged: {
                    console.log("User CB index changed: ", currentIndex)
                    if(currentIndex !== -1) {
                        timeStatusCore.setCurrentUser(usersModel.get(currentIndex).username)
                        timeStatusCore.onDataUpdated()
                    }
                }
            }

            ListModel {
                id: usersModel
            }

            Component.onCompleted: {
                console.log("QML CALL SET USERS COMBOBOX")
                timeStatusCore.getUsersForComboBox()
            }
        }
    }

    Component {
        id: timeStatusLabelComp

        Label {
            anchors {
                left: parent.left
                bottom: parent.bottom
            }
            text: "Time Status"
            font.pixelSize: 18
            font.bold: true
        }
    }
}
