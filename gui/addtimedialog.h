#ifndef ADDTIMEDIALOG_H
#define ADDTIMEDIALOG_H

#include <QObject>
#include "database/mydatabase.h"

class AddTimeDialog : public QObject
{
    Q_OBJECT

public:
    explicit AddTimeDialog(QObject *parent = nullptr);

    ~AddTimeDialog();

    Q_INVOKABLE bool buttonAddTimeClicked(const QString &person,
                                          const QString &date,
                                          const QString &time,
                                          const QString &project,
                                          const QString &description,
                                          bool edit);
    Q_INVOKABLE void setProjectValues(const QString &user);

signals:
    void clearProjects();
    void newProjectReceived(QString projectName);
};

#endif // ADDTIMEDIALOG_H
