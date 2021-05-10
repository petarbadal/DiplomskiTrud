#include "assignproject.h"

AssignProject::AssignProject(QObject *parent) : QObject(parent) {
    connect(MyDatabase::instance(), SIGNAL(setComboBoxUsers(QString)), this, SIGNAL(newUserReceived(QString)));
    setValues();
}

void AssignProject::setValues() {
    emit clearAllData();

    if(!MyDatabase::instance()->getUsersQuery())
        qDebug() << "Fail to setup Workers ComboBox...";
}

bool AssignProject::buttonAssignProjectClicked(const QString &projectName,
                                               const QString &projectDescription,
                                               const QString &clientName,
                                               const QString &companyName,
                                               const QString &startDate,
                                               const QString &endDate,
                                               const QString &projectWorker) {
    if(MyDatabase::instance()->projectNameCheckQuery(projectName))
    {
        if(MyDatabase::instance()->createProjectsRowQuery(projectName, projectDescription, startDate, endDate, companyName, clientName, projectWorker))
        {
            qDebug()<<"Successfuly added project...";

            return true;
        }
    }

    return false;
}
