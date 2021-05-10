#include "addtimedialog.h"

AddTimeDialog::AddTimeDialog(QObject *parent) : QObject(parent) {
    connect(MyDatabase::instance(), SIGNAL(setCheckBoxProjects(QString)), this, SIGNAL(newProjectReceived(QString)));
}

bool AddTimeDialog::buttonAddTimeClicked(const QString &person, const QString &date, const QString &time, const QString &project, const QString &description, const bool edit) {
    if(!edit) {
        if(MyDatabase::instance()->createReportsRowQuery(person, project, time, description, date)) {
            qDebug()<<"Successfuly added report...";
            return true;
        }
    } else if(edit) {
        if(MyDatabase::instance()->updateReportsRowQuery(person, project, time, description, date)) {
            qDebug()<<"Successfuly edited report...";
            return true;
        }
    }

    return false;
}

void AddTimeDialog::setProjectValues(const QString &user) {
    emit clearProjects();
    MyDatabase::instance()->getAllUserProjects(user);
}
