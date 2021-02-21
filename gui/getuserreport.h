#ifndef GETUSERREPORT_H
#define GETUSERREPORT_H

#include "database/mydatabase.h"


class GetUserReport : public QObject
{
    Q_OBJECT

public:
    explicit GetUserReport(QObject *parent = nullptr);

    ~GetUserReport();

    Q_INVOKABLE bool onButtonSaveReportClicked(QString filePath, QString data);
    Q_INVOKABLE void setProjectValues(const QString &user);
    Q_INVOKABLE void getReportDetails(const QString user, const QString project, const QString startDate, const QString endDate);
    Q_INVOKABLE void getUsersForComboBox();

public slots:
    void onSetReportDetails(QString user, QString project, QString startDate, QString endDate);

signals:
    void newProjectReceived(QString projectName);
    void newReportReceived(QString reportDate, QString projectName, QString desc, QString spendTime);
    void clearProjects();
    void clearUsers();
    void clearReports();
};

#endif // GETUSERREPORT_H
