import QtQuick 2.0

Item {
    property ListModel projectsModel: projectsModel

    function createProjectObjects(projectName, row) {
        var component = Qt.createComponent("ProjectObject.qml");
        var sprite = component.createObject(root, {projectName: projectName});

        projectsModel.insert(row, sprite)

        timeStatusCore.setupProjectsHours(projectName, row)
    }

    function setProjectHours(date, value, j) {
        console.log("In setProjectHours QML", date, value, j)

        projectsController.projectsModel.get(j)[setCorrectDayOfWeek(date)] = value
        projectsController.projectsModel.get(j).total += value
    }

    function setCorrectDayOfWeek(numberOfDay) {
        if(numberOfDay === 1)
            return "monday"
        else if(numberOfDay === 2)
            return "tuesday"
        else if(numberOfDay === 3)
            return "wednesday"
        else if(numberOfDay === 4)
            return "thursday"
        else if(numberOfDay === 5)
            return "friday"
        else if(numberOfDay === 6)
            return "saturday"
        else if(numberOfDay === 7)
            return "sunday"
    }

    ListModel {
        id: projectsModel
    }
}
