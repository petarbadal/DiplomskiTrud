#include "assignproject.h"

AssignProject::AssignProject(QObject *parent) :
    QObject(parent)
{
    connect(MyDatabase::instance(), SIGNAL(setComboBoxUsers(QString)), this, SLOT(onNewUserReceived(QString)));
    setValues();
}

AssignProject::~AssignProject()
{}

void AssignProject::setValues()
{
    emit clearAllData();

    if(!MyDatabase::instance()->getUsersQuery())
        qDebug() << "Fail to setup Workers ComboBox...";
}

bool AssignProject::buttonAssignProjectClicked(const QString projectName, const QString projectDescription, const QString clientName, const QString companyName, const QString startDate, const QString endDate, const QString projectWorker)
{
    //qDebug() << projectName << projectDescription << clientName << companyName << startDate << endDate << projectWorker;

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

void AssignProject::onNewUserReceived(QString user)
{
    //qDebug() << Q_FUNC_INFO << user;

    emit newUserReceived(user);
}
