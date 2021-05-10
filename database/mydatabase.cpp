#include "mydatabase.h"

MyDatabase* MyDatabase::m_instance = NULL;

static const QString databasePath(QString::fromStdString(__FILE__) + "\\..\\..\\DB\\main\\time_logger_db.sqlite");

static const QString getWorkerUserQuery("SELECT COUNT(1) FROM workers WHERE workeruser = '%1'");
static const QString checkLoginPassQuery("SELECT password, isadmin FROM workers WHERE workeruser = '%1'");
static const QString createWorkersTableQuery("CREATE TABLE IF NOT EXISTS workers (workeruser varchar(20) primary key not null unique, name varchar(20), surname varchar(20), password varchar(50), isadmin integer(1))");
static const QString createProjectsTableQuery("CREATE TABLE IF NOT EXISTS projects (projectname varchar(20) primary key not null unique, projectdescription varchar(200), startdate date, enddate date, companyname varchar(50), clientname varchar(30), projectworker varchar(20))");
static const QString createReportsTableQuery("CREATE TABLE IF NOT EXISTS reports (wuser varchar(20) references workers(workeruser), pname varchar(20) references projects(projectname), spendtime int4, description varchar(200), reportdate date, constraint pkreport primary key (wuser, pname, reportdate))");

static const QString getAllWorkersQuery("SELECT workeruser FROM workers");
static const QString getAllReportsQuery("SELECT reportdate, pname, description, spendtime FROM reports WHERE wuser=='%1' AND reportdate BETWEEN date('%2') AND date('%3')");
static const QString getAllProjectsQuery("SELECT projectname FROM projects WHERE projectworker=='%1'");
static const QString getProjectByNameQuery("SELECT projectname FROM projects WHERE projectname=='%1'");
static const QString getDonutChartQuery("SELECT pname, sum(spendtime) FROM reports WHERE wuser=='%1' AND reportdate BETWEEN date('%2') AND date('%3') GROUP BY pname");
static const QString getTotalTimeOnProject("SELECT sum(spendtime) FROM reports WHERE wuser=='%1' AND reportdate BETWEEN date('%2') AND date('%3')");
static const QString getBarChartQuery("SELECT spendtime, reportdate FROM reports WHERE wuser=='%1' AND reportdate BETWEEN date('%2') AND date('%3')");
static const QString getTotalHoursQuery("SELECT reportdate, sum(spendtime) FROM reports WHERE pname=='%1' AND wuser=='%2' AND reportdate BETWEEN date('%3') AND date('%4') GROUP BY reportdate");

static const QString deleteReportRowQuery("DELETE FROM reports WHERE wuser=='%1' AND pname='%2' AND reportdate='%3'");

static const QString insertNewReportRowQuery("INSERT INTO reports VALUES ('%1','%2','%3','%4','%5')");
static const QString updateReportRowQuery("UPDATE reports SET(spendtime, description)=('%1','%2') WHERE wuser=='%3' AND pname=='%4' AND reportdate=='%5'");

MyDatabase::MyDatabase(QObject *parent) : QObject(parent)
{}

MyDatabase* MyDatabase::instance() {
    if(!m_instance)
        m_instance = new MyDatabase();

    return m_instance;
}

void MyDatabase::destroyInstance() {
    if (m_instance != nullptr) {
        delete m_instance;
        m_instance = nullptr;
    }
}

int MyDatabase::getIsUser() const {
    return m_isUser;
}

void MyDatabase::setIsUser(int isUser) {
    m_isUser = isUser;
}

bool MyDatabase::checkUserNameQuery(const QString &userName) {
    QSqlQuery query(m_database);

    if(!query.exec(getWorkerUserQuery.arg(userName))) {
        qDebug() << "ERROR : unable to get USER NAME information from workers table";
        return false;
    }

    if(query.next())
        return query.value(0).toBool();

    return false;
}

bool MyDatabase::getUsersQuery() {
    QSqlQuery query(m_database);

    if(!query.exec(getAllWorkersQuery))
        qDebug()<<"ERROR : unable to get workers from workers table";
    else {
        while(query.next()) {
            QString text(query.value(0).toString());
            if(!text.isEmpty())
                emit setComboBoxUsers(text);
        }
        return true;
    }
    return false;
}

bool MyDatabase::openConnection() {
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(databasePath);
    qDebug() << "DB Path: " << m_database.databaseName();

    if(!m_database.open()) {
        qDebug()<<"ERROR : "<< m_database.lastError().text();
        return false;
    }

    return true;
}

void MyDatabase::closeConnection() {
    m_database.removeDatabase(QSqlDatabase::defaultConnection);
    m_database.close();
}
void MyDatabase::setCurrentUser(const QString &user) {
    m_currentUser = user;
}

QString MyDatabase::getCurrentUser() {
    return m_currentUser;
}

int MyDatabase::getAction() const {
    return m_action;
}

void MyDatabase::setAction(int value) {
    m_action = value;
}

bool MyDatabase::createTablesQuery() {
    QSqlQuery query(m_database);

    if(query.exec(createWorkersTableQuery))
        qDebug()<<"Table workers is available!";
    else
        return false;

    query.clear();
    if(query.exec(createProjectsTableQuery))
        qDebug()<<"Table projects is available!";
    else
        return false;

    query.clear();
    if(query.exec(createReportsTableQuery))
        qDebug()<<"Table reports is available!";
    else
        return false;

    return true;
}

bool MyDatabase::signUpCheckQuery(const QString &workeruser, const QString &name, const QString &surname, const QString &password, const bool isAdmin) {
    QSqlQuery queryAvailable(m_database);

    QByteArray cryptedPassword;
    cryptedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);

    if(!queryAvailable.exec(getWorkerUserQuery.arg(workeruser)))
        qDebug()<<"ERROR : unable to get name of worker from workers table";
    else {
        if(!queryAvailable.value(0).toBool()) {
            QSqlQuery queryFillInTable(m_database);
            queryFillInTable.prepare("INSERT INTO workers VALUES ('" + workeruser + "','" + name + "','" + surname + "','" + cryptedPassword + "','" + isAdmin + "'");
            if(queryFillInTable.exec()) {
                qDebug()<<"Successfuly signed up!";
                return true;
            }
        }
    }
    return false;
}

bool MyDatabase::logInCheckQuery(const QString &user, const QByteArray &cryptedPassword) {
    QSqlQuery query(m_database);
    if(!query.exec(checkLoginPassQuery.arg(user))) {
        qDebug() << "ERROR : unable to get login information from workers table";
        return false;
    } else if(query.next()) {
        if(query.value(0) == cryptedPassword) {
            MyDatabase::instance()->setCurrentUser(user);
            MyDatabase::instance()->setIsAdmin(query.value(1).toBool());
            return true;
        }
        else
            qDebug() << "Did not match!";
    }

    return false;
}

bool MyDatabase::setDonutChartQuery(const QDate &firstDayOfWeek, const QDate &lastDayOfWeek) {
    QSqlQuery getValues(m_database);

    if(!getValues.exec(getDonutChartQuery.arg(m_currentUser, firstDayOfWeek.toString("yyyy-MM-dd"), lastDayOfWeek.toString("yyyy-MM-dd"))))
        qDebug()<<"ERROR : Setting the donut chart failed!";
    else {
        int counter = 0;
        QSqlQuery getSum(m_database);

        if(!getSum.exec(getTotalTimeOnProject.arg(m_currentUser, firstDayOfWeek.toString("yyyy-MM-dd"), lastDayOfWeek.toString("yyyy-MM-dd"))))
            qDebug()<<"ERROR : Setting donut chart failed!";
        else {
            getSum.first();
            counter = getSum.value(0).toInt();
            emit updateHours(getSum.value(0).toString() == 0 ? "0" : getSum.value(0).toString());
        }

        while(getValues.next()) {
            double br = (getValues.value(1).toDouble() / counter) * 100;

            QString show;
            show.append(getValues.value(0).toString() + "\n");
            show.append(getValues.value(1).toString() + " hrs");
            show.append("[" + QString::number(int(br)) + "%]");

            setPieValues(show, getValues.value(1).toInt());
        }
        return true;
    }
    return false;
}

bool MyDatabase::setBarChartQuery(const QDate &firstDayOfWeek, const QDate &lastDayOfWeek) {
    QSqlQuery loadHours(m_database);
    if(!loadHours.exec(getBarChartQuery.arg(m_currentUser, firstDayOfWeek.toString("yyyy-MM-dd"), lastDayOfWeek.toString("yyyy-MM-dd"))))
        qDebug()<<"ERROR : Setting bar chart failed!";
    else {
        int monday = 0, tuesday = 0, wednesday = 0, thursday = 0, friday = 0, saturday = 0, sunday = 0;

        while(loadHours.next()) {
            QDate firstDate = firstDayOfWeek;

            if(loadHours.value(1).toString() == firstDate.toString("yyyy-MM-dd")) {
                monday += loadHours.value(0).toInt();
                continue;
            }

            firstDate = firstDate.addDays(1);
            if(loadHours.value(1).toString() == firstDate.toString("yyyy-MM-dd")) {
                tuesday += loadHours.value(0).toInt();
                continue;
            }

            firstDate = firstDate.addDays(1);
            if(loadHours.value(1).toString() == firstDate.toString("yyyy-MM-dd")) {
                wednesday += loadHours.value(0).toInt();
                continue;
            }

            firstDate = firstDate.addDays(1);
            if(loadHours.value(1).toString() == firstDate.toString("yyyy-MM-dd")) {
                thursday += loadHours.value(0).toInt();
                continue;
            }

            firstDate = firstDate.addDays(1);
            if(loadHours.value(1).toString() == firstDate.toString("yyyy-MM-dd")) {
                friday += loadHours.value(0).toInt();
                continue;
            }

            firstDate = firstDate.addDays(1);
            if(loadHours.value(1).toString() == firstDate.toString("yyyy-MM-dd")) {
                saturday += loadHours.value(0).toInt();
                continue;
            }

            firstDate = firstDate.addDays(1);
            if(loadHours.value(1).toString() == firstDate.toString("yyyy-MM-dd")) {
                sunday += loadHours.value(0).toInt();
                continue;
            }
        }
        emit setDays(monday, tuesday, wednesday, thursday, friday, saturday, sunday);

        return true;
    }
    return false;
}

bool MyDatabase::getProjectCountQuery()
{
    bool isOkay = false;
    QSqlQuery getSize(m_database);
    getSize.prepare("select count(projectname) from projects where projectworker == '"+MyDatabase::instance()->getCurrentUser()+"'");
    if(!getSize.exec()){
        qDebug()<<"ERROR : couldn't count the projects";
    }
    else{
        getSize.first();
        m_projectCount = getSize.value(0).toInt() + 1;
        isOkay = true;
    }
    return isOkay;
}

bool MyDatabase::getReportsCountQuery(QDate firstDayOfWeek, QDate lastDayOfWeek)
{
    bool isOkay = false;
    //Query that counts the number of reports this week from the user so it can set the row count of the table
    QSqlQuery getSize(m_database);
    getSize.prepare("select count(description) from reports where wuser=='"+MyDatabase::instance()->getCurrentUser()+"'and reportdate BETWEEN date('"+firstDayOfWeek.toString("yyyy-MM-dd")+"') and date('"+lastDayOfWeek.toString("yyyy-MM-dd")+"')");
    m_reportsCount = 0;
    if(!getSize.exec()){
        qDebug()<<"ERROR : couldn't count the reports";
    }
    else
    {
        getSize.first();
        m_reportsCount = getSize.value(0).toInt() + 1;
        isOkay = true;
    }
    return isOkay;
}

bool MyDatabase::getProjectQuery() {
    QSqlQuery getProjects(m_database);

    if(!getProjects.exec(getAllProjectsQuery.arg(m_currentUser)))
        qDebug()<<"ERROR : unable to get projects from projects table";
    else {
        int i = 0;
        while(getProjects.next()) {
            const QString &text = getProjects.value(0).toString();
            emit setProjects(text, i);

            i++;
        }

        return true;
    }
    return false;
}

bool MyDatabase::getReportsQuery(const QDate &firstDayOfWeek, const QDate &lastDayOfWeek) {
    QSqlQuery loadReport(m_database);

    if(!loadReport.exec(getAllReportsQuery.arg(m_currentUser, firstDayOfWeek.toString("yyyy-MM-dd"), lastDayOfWeek.toString("yyyy-MM-dd"))))
        qDebug()<<"ERROR : unable to get reports from reports table";
    else {
        while(loadReport.next()) {
            const QString &reportDate = loadReport.value("reportdate").toString();
            const QString &projectName = loadReport.value("pname").toString();
            const QString &desc = loadReport.value("description").toString();
            const QString &spendTime = loadReport.value("spendtime").toString();

            emit setReports(reportDate, projectName, desc, spendTime);
        }
        return true;
    }
    return false;
}

bool MyDatabase::getHoursQuery(const QString &projectName, const QDate &firstDayOfWeek, const QDate &lastDayOfWeek, const int j) {
    QSqlQuery returnHours(m_database);

    if(!returnHours.exec(getTotalHoursQuery.arg(projectName, m_currentUser, firstDayOfWeek.toString("yyyy-MM-dd"), lastDayOfWeek.toString("yyyy-MM-dd"))))
        qDebug()<<"ERROR : unable to return hours from reports table";
    else {
        while(returnHours.next())
            emit setHours(returnHours.value(0).toDate().dayOfWeek(), returnHours.value(1).toInt(), j);
        return true;
    }
    return false;
}

bool MyDatabase::deleteRowReportsQuery(const QString &user, const QString &project, const QString &date) {
    QSqlQuery queryDeleteRow(m_database);

    if(!queryDeleteRow.exec(deleteReportRowQuery.arg(user, project, date)))
        qDebug()<<"ERROR : deleting the row failed";
    else {
        emit dataUpdated();

        return true;
    }

    return false;
}

bool MyDatabase::projectNameCheckQuery(const QString &projectName) {
    QSqlQuery query(m_database);
    if(!query.exec(getProjectByNameQuery.arg(projectName)))
        qDebug()<<"ERROR : unable to get projects from projects table";
    else {
        if(query.next())
            return false;
    }

    return true;
}

bool MyDatabase::createProjectsRowQuery(QString projectName, QString projectDescription, QString startDate, QString endDate, QString companyName, QString clientName, QString projectWorker)
{
    bool isOkay = false;
    QSqlQuery queryFillInTable(m_database);
    queryFillInTable.prepare("insert into projects values ('"+projectName+"','"+projectDescription+"','"+startDate+"','"+endDate+"','"+companyName+"','"+clientName+"','"+projectWorker+"');");
    if(queryFillInTable.exec())
    {
        isOkay = true;
    }
    else
    {
        qDebug()<<"ERROR : inserting row into projects failed";
    }
    return isOkay;
}

bool MyDatabase::createReportsRowQuery(const QString &person, const QString &project, const QString &time, const QString &description, const QString &date) {
    QSqlQuery queryFillInTable(m_database);

    if(!queryFillInTable.exec(insertNewReportRowQuery.arg(person, project, time, description, date)))
        qDebug()<<"ERROR : inserting row into reports failed";
    else {
        emit dataUpdated();
        return true;
    }

    return false;
}

bool MyDatabase::updateReportsRowQuery(const QString &person, const QString &project, const QString &time, const QString &description, const QString &date) {
    QSqlQuery queryFillInTable(m_database);

    if(!queryFillInTable.exec(updateReportRowQuery.arg(time, description, person, project, date)))
        qDebug()<<"ERROR : updating row in reports failed";
    else
        return true;

    return false;
}

int MyDatabase::getAllReportsCount() const
{
    return m_allReportsCount;
}

bool MyDatabase::getAllUserReportsQuery(QString user, QString projectName, QString startDate, QString endDate)
{
    bool isOkay = false;
    //Query that selects all the information from the report table that is needed to be displayed in the table
    QSqlQuery loadReport(m_database);
    if(projectName == "All Projects")
    {
        loadReport.prepare("select reportdate,pname,description,spendtime from reports where wuser=='"+user+"'AND reportdate BETWEEN '"+startDate+"' and '"+endDate+"'");
    }
    else
        loadReport.prepare("select reportdate,pname,description,spendtime from reports where wuser=='"+user+"' AND pname=='"+projectName+"'");
    if(!loadReport.exec())
    {
        qDebug()<<"ERROR : unable to get reports from reports table";
    }
    else
    {
        while(loadReport.next())
        {
            QString reportDate = loadReport.value("reportdate").toString();
            QString projectName = loadReport.value("pname").toString();
            QString desc = loadReport.value("description").toString();
            QString spendTime = loadReport.value("spendtime").toString();

            emit setReportsInGetUserDialog(reportDate, projectName, desc, spendTime);
        }
        isOkay = true;
    }
    return isOkay;
}

bool MyDatabase::getAllUserReportsCountQuery(QString user, QString projectName, QString startDate, QString endDate)
{
    bool isOkay = false;
    //Query that counts the number of reports this week from the user so it can set the row count of the table
    QSqlQuery getSize(m_database);
    if(projectName == "All Projects")
    {
        getSize.prepare("select count(description) from reports where wuser=='"+user+"' and reportdate BETWEEN '"+startDate+"' and '"+endDate+"'");
    }
    else
        getSize.prepare("select count(description) from reports where wuser=='"+user+"' AND pname=='"+projectName+"' AND reportdate BETWEEN '"+startDate+"' and '"+endDate+"'");
    m_reportsCount = 0;
    if(!getSize.exec()){
        qDebug()<<"ERROR : couldn't count the reports";
    }
    else
    {
        getSize.first();
        m_allReportsCount = getSize.value(0).toInt() + 1;
        isOkay = true;
    }
    return isOkay;
}

int MyDatabase::getReportsCount() const
{
    return m_reportsCount;
}

bool MyDatabase::getAllUserProjects(const QString &user) {
    QSqlQuery getProjects(m_database);

    if(!getProjects.exec(getAllProjectsQuery.arg(user)))
        qDebug()<<"ERROR : unable to get all user projects from projects table";
    else {
        while(getProjects.next()) {
            const QString &text = getProjects.value(0).toString();
            emit setCheckBoxProjects(text);
        }
        return true;
    }
    return false;
}

bool MyDatabase::getWorkerProjects(QString workerName)
{
    bool isOkay = false;
    QSqlQuery queryGetWorkerProjects(m_database);
    queryGetWorkerProjects.prepare("select pname from projects where wuser == '"+workerName+"'");
    if(queryGetWorkerProjects.exec())
    {
        isOkay = true;
        emit workerProjects(queryGetWorkerProjects.value(0).toString());
    }
    else {
        qDebug()<<"ERROR : updating row in reports failed";
    }
    return isOkay;
}

bool MyDatabase::getIsAdmin() const
{
    return m_isAdmin;
}

void MyDatabase::setIsAdmin(bool isAdmin)
{
    qDebug() << Q_FUNC_INFO << isAdmin;
    m_isAdmin = isAdmin;
    emit updateIsAdmin(m_isAdmin);
}

int MyDatabase::getProjectCount() const
{
    return m_projectCount;
}
