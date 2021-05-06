#include "getuserreport.h"

GetUserReport::GetUserReport(QObject *parent) : QObject(parent)
{
    connect(MyDatabase::instance(), SIGNAL(setComboBoxUsers(QString)), this, SIGNAL(newProjectReceived(QString)));
    connect(MyDatabase::instance(), SIGNAL(setReportsInGetUserDialog(QString, QString, QString, QString)), this, SLOT(onSetReportDetails(QString, QString, QString, QString)));
}

GetUserReport::~GetUserReport()
{}

void GetUserReport::setProjectValues(const QString &user)
{
    emit clearProjects();
    MyDatabase::instance()->getAllUserProjects(user);
}

void GetUserReport::onSetReportDetails(QString reportDate, QString projectName, QString desc, QString spentTime)
{
    //qDebug() << Q_FUNC_INFO << reportDate << projectName << desc << spentTime;
    emit newReportReceived(reportDate, projectName, desc, spentTime);
}

void GetUserReport::getReportDetails(const QString user, const QString project, const QString startDate, const QString endDate)
{
    //qDebug() << Q_FUNC_INFO << user << project << startDate << endDate;
    emit clearReports();
    MyDatabase::instance()->getAllUserReportsQuery(user, project, startDate, endDate);
}

void GetUserReport::getUsersForComboBox()
{
    emit clearUsers();

    if(!MyDatabase::instance()->getUsersQuery())
        qDebug() << "Fail to setup Workers ComboBox";
}

bool GetUserReport::onButtonSaveReportClicked(QString filePath, QString data)
{
    bool isSaved = false;

    QFile file(filePath);
    if(file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream stream(&file);
        stream<<"Date, Project Name, Work Description, Hours\n";

        auto splitData = data.split("\n");
        for(int i=0; i<splitData.size(); i++)
        {
            //qDebug() << splitData[i];
            stream << splitData[i] + "\n";
        }

        file.close();
        isSaved = true;
    }

    if(isSaved)
        qDebug()<<"Report Saved!";
    else
        qDebug()<<"Report not saved...";

    return isSaved;
}
