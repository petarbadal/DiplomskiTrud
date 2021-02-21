import QtQuick 2.0

Item {
    property ListModel reportsModel: reportsModel

    function createReportObjects(reportDate, projectName, desc, spendTime) {

        var component = Qt.createComponent("ReportObject.qml");
        var sprite = component.createObject(root, {date: reportDate, project: projectName, description: desc, hours: spendTime});

        reportsModel.append(sprite)
    }

    ListModel {
        id: reportsModel
    }
}
