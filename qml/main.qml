import QtQuick 2.12
import QtQuick.Window 2.12
import QtCharts 2.0

Window {
    id: appWindow

    property bool loginPassed: false

    property var mainWindowObject: 0

    title: qsTr("TeamM")

    onLoginPassedChanged: {
        console.log("Login Process Passed", loginPassed)
        if(loginPassed) {
            appWindow.visible = false
            mainWindowObject = mainWindowComponent.createObject()
        }
        else {
            if(mainWindowObject != 0)
                mainWindowObject.destroy()
            appWindow.visible = true
        }
    }

    function logout() {
        mainWindowObject.destroy()
        loginPassed = false
    }

    visible: true
    width: 300
    height: 455

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    LoginWindow {
        id: loginWindow

        anchors.fill: parent
    }

    Component {
        id: mainWindowComponent

        MainWindow {
            id: mainWindow
        }
    }
}
