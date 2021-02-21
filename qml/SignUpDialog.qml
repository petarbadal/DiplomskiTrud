import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "CommonComponents"

Popup {
    id: root

    property bool enableButton: (nameInput.textField.text.length > 3) &&
                                (surnameInput.textField.text.length > 3) &&
                                (userInput.textField.text.length> 3) &&
                                (passInput.textField.text.length > 3)

    Frame {
        id: mainContentFrame

        clip: true
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: parent.height - 85

        background: Rectangle {
            color: "lightblue"
        }

        ColumnLayout {
            spacing: 3
            anchors.centerIn: parent
            width: 300

            CustomTextInput {
                id: nameInput

                height: 30
                Layout.fillWidth: true
                labelText: "Name"
            }

            CustomTextInput {
                id: surnameInput

                height: 30
                Layout.fillWidth: true
                labelText: "Surname"
            }

            CustomTextInput {
                id: userInput

                height: 30
                Layout.fillWidth: true
                labelText: "User"
            }

            CustomTextInput {
                id: passInput

                height: 30
                Layout.fillWidth: true
                labelText: "Password"
                passwordMode: true
            }

            CustomCheckBox {
                id: signUpAsAdmin

                height: 10
                Layout.leftMargin: 10
                checkBoxText: "Sign Up as Administrator"
                checked: true
            }
        }
    }

    Rectangle {
        anchors {
            topMargin: 5
            top: mainContentFrame.bottom
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }

        width: 310
        height: 75
        color: enableButton ? "#cbe368" : "lightgray"
        border.color: "lightgray"
        border.width: 3

        Text {
            anchors.centerIn: parent
            text: "SIGN UP"
            font.pixelSize: 18
        }

        MouseArea {
            anchors.fill: parent
            enabled: enableButton
            onClicked: {
                if(timeStatusCore.onButtonSingUpClicked(userInput.textField.text,
                                                     nameInput.textField.text,
                                                     surnameInput.textField.text,
                                                     passInput.textField.text,
                                                     signUpAsAdmin.checked))
                    root.close()
            }
        }
    }
}
