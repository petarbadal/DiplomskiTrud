import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import BackendAccess 1.0

import "CommonComponents"

Item {
    id: root

    property bool rememberMeCheck: false
    property bool loginButtonAvailable: ((logInUserPassword != "")  && (logInUserPassword.length > 2))
    property bool nextButtonAvailable: ((logInUserName != "") && (logInUserName.length > 3))

    property string logInUserName: ""
    property string logInUserPassword: ""
    property string notificationText: ""

    property alias loginPageMainLoader: loginMainLoader

    LoginCore {
        id: loginCore
    }

    Rectangle {
        anchors.fill: parent
        color: "lightblue"
    }

    ColumnLayout {
        spacing: 5

        Rectangle {
            width: 280
            height: 170
            color: "white"
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            radius: 60

            Image {
                source: "qrc:/images/images/logo.svg"
                sourceSize.height: 160
                sourceSize.width: 240
                anchors.centerIn: parent
            }
        }

        Rectangle {
            width: 300
            height: 40
            color: "#cbe368"

            Text {
                anchors.centerIn: parent
                text: "Welcome Back"
                font.family: "Segoe Script"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 15
            }
        }

        Loader {
            id: loginMainLoader

            sourceComponent: enterUserNameComp
        }

    }

    Component {
        id: enterUserNameComp

        ColumnLayout {
            id: userNameLayout
            width: 300
            Layout.fillHeight: true
            spacing: 10

            Item {
                width: parent.width
                height: 35
            }

            CustomTextInput {
                width: parent.width
                height: 40
                Layout.alignment: Qt.AlignHCenter
                usage: "userName"
                imageSource: "/images/images/userIcon.png"
                inputText: loginCore.savedUserName === "" ? logInUserName : loginCore.savedUserName
                defaultPlaceholderText: "Enter user name"
                onEnterKeyPressed: {
                    nextButton.clicked()
                }

                Component.onCompleted: {
                    console.log("Input TExt:", inputText, "Saved:", loginCore.savedUserName)
                }
            }

            Item {
                width: parent.width
                height: 35

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text: notificationText
                    font.family: "Segoe UI"
                    font.italic: true
                    color: "red"
                }
            }

            Button {
                id: nextButton

                Layout.alignment: Qt.AlignBottom
                Layout.margins: 5
                enabled: nextButtonAvailable

                background: Rectangle {
                    implicitHeight: 85
                    implicitWidth: root.width - 10
                    color: nextButtonAvailable ? "#cbe368" : "lightgray"
                    border.color: "lightgray"
                    border.width: 3
                }

                text: "NEXT"
                font.pixelSize: 18

                onClicked: {
                    var checkUserName = loginCore.buttonNextClicked(root.logInUserName)
                    if(!checkUserName)
                        notificationText = "Couldn't find your user name"
                    else {
                        notificationText = ""
                        loginMainLoader.sourceComponent = enterPasswordComp
                    }
                }
            }
        }
    }

    Component {
        id: enterPasswordComp

        ColumnLayout {
            width: 300
            Layout.fillHeight: true
            spacing: 10

            Rectangle {
                Layout.leftMargin: 10
                width: 20
                height: 30
                radius: 10
                color: backMA.pressed ?  "white" : "transparent"

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    source: "qrc:/images/images/lA.png"
                    sourceSize.width: 20
                    sourceSize.height: 20
                }

                MouseArea {
                    id: backMA

                    anchors.fill: parent
                    onClicked: {
                        loginMainLoader.sourceComponent = enterUserNameComp
                    }
                }
            }

            CustomTextInput {
                width: parent.width
                height: 40
                Layout.alignment: Qt.AlignHCenter
                usage: "userPassword"
                imageSource: "/images/images/passIcon.png"
                inputText: loginCore.savedUserPass
                passwordMode: true
                defaultPlaceholderText: "Enter your password"

                onEnterKeyPressed: {
                    loginButton.clicked()
                }
            }

            ColumnLayout {
                spacing: 5
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.topMargin: -5

                Item {
                    width: parent.width
                    height: 10

                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        text: notificationText
                        font.family: "Segoe UI"
                        font.italic: true
                        color: "red"
                    }
                }

                CustomCheckBox {
                    checkBoxText: "Remember me!"
                    checked: true
                }
            }

            Button {
                id: loginButton

                Layout.alignment: Qt.AlignBottom
                Layout.topMargin: 10
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                Layout.bottomMargin: 5
                enabled: loginButtonAvailable

                background: Rectangle {
                    implicitHeight: 85
                    implicitWidth: root.width - 10
                    color: loginButtonAvailable ? "#cbe368" : "lightgray"
                    border.color: "lightgray"
                    border.width: 3
                }

                text: "LOGIN"
                font.pixelSize: 18

                onClicked: {
                    var checkPassword = loginCore.buttonLogInClicked(root.logInUserName, root.logInUserPassword)

                    if(!checkPassword) {
                        notificationText = "Wrong password. Try again or contact your admin"
                    }

                    var successfullyRemembered
                    if(rememberMeCheck) {
                        successfullyRemembered = loginCore.saveCreditentials(root.logInUserName, root.logInUserPassword)
                        console.log("User was " + (successfullyRemembered ? "Succesully" : "Unsuccessfully"), "remembered")
                    }
                    else
                        successfullyRemembered = loginCore.deleteCreditentials()

                    loginPassed = checkPassword
                }
            }
        }
    }
}
