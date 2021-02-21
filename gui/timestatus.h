#ifndef TIMESTATUS_H
#define TIMESTATUS_H

#include <QObject>
#include <QDate>

#include "database/mydatabase.h"

class TimeStatus : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString currentUser READ currentUser NOTIFY currentUserChanged)
    Q_PROPERTY(QString firstDay READ firstDay NOTIFY firstDayChanged)
    Q_PROPERTY(QString lastDay READ lastDay NOTIFY lastDayChanged)
    Q_PROPERTY(QString year READ year NOTIFY yearChanged)
    Q_PROPERTY(QString hoursSum READ hoursSum NOTIFY hoursSumChanged)
    Q_PROPERTY(bool isAdmin READ isAdmin WRITE setIsAdmin NOTIFY isAdminChanged)

    QString currentUser() const {
        return currentUser_;
    }

    QString firstDay() {
        return m_firstDayOfWeek.toString("MMMM dd");
    }

    QString lastDay() {
        return m_lastDayOfWeek.toString("MMMM dd");
    }

    QString year() {
        return m_firstDayOfWeek.toString("yyyy");
    }

    QString hoursSum() {
        return weeklyHoursSum_;
    }

    bool isAdmin() {
        return isAdmin_;
    }

    explicit TimeStatus(QObject *parent = nullptr);

    ~TimeStatus();
    Q_INVOKABLE void setupGuiData();
    Q_INVOKABLE void setupProjectsHours(const QString &projectName, const int row);
    Q_INVOKABLE void buttonNextWeekClicked();
    Q_INVOKABLE void buttonPreviousWeekClicked();
    Q_INVOKABLE void onButtonDeleteClicked(const QString &project, const QString &dateString);
    Q_INVOKABLE bool onButtonSingUpClicked(const QString &user, const QString &name, const QString &surname, const QString &password, bool signupAsAdmin);
    Q_INVOKABLE void getUsersForComboBox();
    Q_INVOKABLE void setCurrentUser(const QString &name);

public slots:
    void onDataUpdated();
    void setIsAdmin(bool isAdmin);
    void onUpdatedHours(const QString &hours);

signals:
    void isAdminChanged(bool isAdmin);
    void currentUserChanged(QString currentUser);
    void firstDayChanged(QString firstDay);
    void lastDayChanged(QString lastDay);
    void yearChanged(QString year);
    void hoursSumChanged(QString hours);

    void newReportReceived(QString reportDate, QString projectName, QString desc, QString spendTime);
    void newProjectReceived(QString project, int row);
    void newProjectHoursReceived(int date, int value, int j);
    void newPieValueReceived(QString projectInfo, int hours);
    void newBarValueReceived(int monday, int tuesday, int wednesday, int thursday, int friday, int saturday, int sunday);
    void newComboBoxUserReceived(QString user);

    void clearAllData();
    void clearAllUsersComboBoxData();

private:
    void setDonutChart();
    void setBarChart();

    QDate m_firstDayOfWeek;
    QDate m_lastDayOfWeek;

    int m_year;
    int m_month;
    int m_day;
    int m_j;

    QString currentUser_;
    QString weeklyHoursSum_;

    bool isAdmin_;
};

#endif // TIMESTATUS_H
