#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlQuery>

class MyDatabase : public QObject
{
    Q_OBJECT
public:
    static MyDatabase *instance();
    static void destroyInstance();

    bool getUsersQuery(int flag);

    void closeConnection();
    bool openConnection();

    void setCurrentUser(QString user);
    QString getCurrentUser();

    int getAction() const;
    void setAction(int value);

    bool createTablesQuery();

    bool signUpCheckQuery(QString workeruser, QString name, QString surname, QString password, bool isAdmin);

    bool logInCheckQuery(QString user,QByteArray cryptedPassword);

    bool setDonutChartQuery(QDate firstDayOfWeek, QDate lastDayOfWeek);

    bool setBarChartQuery(QDate firstDayOfWeek,QDate lastDayOfWeek);

    bool getHoursQuery(QString projectName,QDate firstDayOfWeek,QDate lastDayOfWeek,int j);

    bool getProjectQuery(int flag);
    bool getProjectCountQuery();
    bool createProjectsRowQuery(QString projectName,QString projectDescription,QString startDate,QString endDate,QString companyName,QString clientName,QString projectWorker);
    bool projectNameCheckQuery(QString projectName);

    bool getReportsQuery(QDate firstDayOfWeek,QDate lastDayOfWeek);
    bool getReportsCountQuery(QDate firstDayOfWeek,QDate lastDayOfWeek);

    bool createReportsRowQuery(const QString &person, const QString &project, const QString &time, const QString &description, const QString &date);
    bool updateReportsRowQuery(const QString &person, const QString &project, const QString &time, const QString &description, const QString &date);

    bool deleteRowReportsQuery(QString user,QString project,QString date);
    bool getAllUserReportsCountQuery(QString user, QString projectName, QString startDate, QString endDate);

    int getProjectCount() const;
    int getReportsCount() const;

    bool getIsAdmin() const;
    void setIsAdmin(bool isAdmin);

    bool getWorkerProjects(QString workerName);

    int getAllReportsCount() const;

    bool getAllUserReportsQuery(QString user, QString projectName, QString startDate, QString endDate);

    bool getAllUserProjects(QString user);

    int getIsUser() const;
    void setIsUser(int isUser);

    bool checkUserNameQuery(const QString &userName);

signals:
    void setPieValues(QString projecName, int hours);
    void updateHours(QString hours);
    void updateHoursAdmin(QString hours);
    void setDays(int monday,int tuesday, int wednesday, int thursday, int friday, int saturday, int sunday);
    void setDaysAdmin(int monday,int tuesday, int wednesday, int thursday, int friday, int saturday, int sunday);
    void setProjects(QString project,int i);
    void setProjectsAdmin(QString project,int i);
    void setHours(int date, int value, int j);
    void setHoursAdmin(QString date,QString value,int j);
    void setReports(QString reportDate, QString projectName, QString desc, QString spentTime);
    void setReportsAdmin(int m, int k, QString text);
    void setCheckBox(QString text);
    void dataUpdated();
    void addButtons(int m);
    void addButtonsAdmin(int m);
    void addToolTip(int m);
    void workerProjects(QString projectName);
    void setCheckBoxProjects(QString text);
    void dataUpdatedAdmin();
    void setReportsInGetUserDialog(QString reportDate, QString projectName, QString desc, QString spentTime);
    void setComboBoxUsers(QString user);
    void setComboBoxUsersMainW(QString user);
    void setComboBoxGetUserReport(QString user);
    void updateIsAdmin(bool isAdmin);

private:
    explicit MyDatabase(QObject *parent = nullptr);
    MyDatabase(const MyDatabase&);
    MyDatabase& operator=(const MyDatabase&);
    static MyDatabase *m_instance;
    QSqlDatabase m_myDatabase;
    QString m_currentUser;
    bool m_isAdmin;
    int m_isUser;
    int m_action;
    int m_projectCount;
    int m_reportsCount;
    int m_allReportsCount;
};

#endif // MYDATABASE_H
