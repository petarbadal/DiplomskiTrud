#ifndef ASSIGNPROJECT_H
#define ASSIGNPROJECT_H

#include <QObject>
#include "database/mydatabase.h"


class AssignProject : public QObject
{
    Q_OBJECT

public:
    explicit AssignProject(QObject *parent = nullptr);
    ~AssignProject();

    Q_INVOKABLE void setValues();

    Q_INVOKABLE bool buttonAssignProjectClicked(const QString projectName,
                                                const QString projectDescription,
                                                const QString clientName,
                                                const QString companyName,
                                                const QString startDate,
                                                const QString endDate,
                                                const QString projectWorker);
private slots:
    void onNewUserReceived(QString user);

signals:
    void dataUpdatedAdmin();
    void newUserReceived(QString user);
    void clearAllData();
};

#endif // ASSIGNPROJECT_H
