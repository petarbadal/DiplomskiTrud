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

    void closeConnection();
    bool openConnection();

    bool getUsersQuery();

    void setCurrentUser(const QString &user);
    QString getCurrentUser();

    int getAction() const;
    void setAction(int value);

    bool createTablesQuery();

    bool signUpCheckQuery(const QString &workeruser, const QString &name, const QString &surname, const QString &password, const bool isAdmin);

    bool logInCheckQuery(const QString &user, const QByteArray &cryptedPassword);

    bool setDonutChartQuery(const QDate &firstDayOfWeek, const QDate &lastDayOfWeek);

    bool setBarChartQuery(const QDate &firstDayOfWeek, const QDate &lastDayOfWeek);

    bool getHoursQuery(const QString &projectName, const QDate &firstDayOfWeek, const QDate &lastDayOfWeek, const int j);

    bool getProjectQuery();
    bool getProjectCountQuery();
    bool createProjectsRowQuery(QString projectName,QString projectDescription,QString startDate,QString endDate,QString companyName,QString clientName,QString projectWorker);
    bool projectNameCheckQuery(const QString &projectName);

    bool getReportsQuery(const QDate &firstDayOfWeek, const QDate &lastDayOfWeek);
    bool getReportsCountQuery(QDate firstDayOfWeek,QDate lastDayOfWeek);

    bool createReportsRowQuery(const QString &person, const QString &project, const QString &time, const QString &description, const QString &date);
    bool updateReportsRowQuery(const QString &person, const QString &project, const QString &time, const QString &description, const QString &date);

    bool deleteRowReportsQuery(const QString &user, const QString &project, const QString &date);
    bool getAllUserReportsCountQuery(QString user, QString projectName, QString startDate, QString endDate);

    int getProjectCount() const;
    int getReportsCount() const;

    bool getIsAdmin() const;
    void setIsAdmin(bool isAdmin);

    bool getWorkerProjects(QString workerName);

    int getAllReportsCount() const;

    bool getAllUserReportsQuery(QString user, QString projectName, QString startDate, QString endDate);

    bool getAllUserProjects(const QString &user);

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

    void dataUpdated();

    void addButtonsAdmin(int m);

    void workerProjects(QString projectName);
    void setCheckBoxProjects(QString text);
    void dataUpdatedAdmin();
    void setReportsInGetUserDialog(QString reportDate, QString projectName, QString desc, QString spentTime);
    void updateIsAdmin(bool isAdmin);

    void setComboBoxUsers(const QString &user);

private:
    explicit MyDatabase(QObject *parent = nullptr);
    MyDatabase(const MyDatabase&);
    MyDatabase& operator=(const MyDatabase&);
    static MyDatabase *m_instance;
    QSqlDatabase m_database;
    QString m_currentUser;
    bool m_isAdmin;
    int m_isUser;
    int m_action;
    int m_projectCount;
    int m_reportsCount;
    int m_allReportsCount;
};

#endif // MYDATABASE_H
