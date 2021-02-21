#include "mydatabase.h"

#include <QtCore/QDebug>

MyDatabase* MyDatabase::m_instance = NULL;

MyDatabase::MyDatabase(QObject *parent) : QObject(parent)
{}

//Return DB Instance
MyDatabase* MyDatabase::instance()
{
    if(!m_instance)
        m_instance = new MyDatabase();
    return m_instance;
}
//Delete Instance
void MyDatabase::destroyInstance()
{
    if (m_instance != nullptr)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}
//Get and set IsUser are used when user with admin privilege logs in as Normal user
int MyDatabase::getIsUser() const
{
    return m_isUser;
}

void MyDatabase::setIsUser(int isUser)
{
    qDebug() << "User: " << isUser;
    m_isUser = isUser;
}

bool MyDatabase::checkUserNameQuery(const QString &userName)
{
    bool isOkay = false;

    QSqlQuery queryAuthentication(m_myDatabase);
    queryAuthentication.prepare("select workeruser from workers;");

    if(!queryAuthentication.exec())
    {
        qDebug()<<"ERROR : unable to get USER NAME information from workers table";
    }
    else
    {
        while(queryAuthentication.next())
        {
            if((queryAuthentication.value(0) == userName))
            {
                //if we find a match we show the time status window and update the data shown there to be the most recent one
                qDebug()<<"Found user name: " << userName;
                isOkay = true;
                break;
            }
            else {
                qDebug()<<"Did not found user: " << userName;
            }
        }
    }
    return isOkay;
}


//This Query return String List of all Users in Database
bool MyDatabase::getUsersQuery(int flag)
{
    bool isOkay=false;
    //Query for getting the name of the user so it can be set to the table
    QSqlQuery getWorkers(m_myDatabase);
    getWorkers.prepare("select workeruser from workers");
    if(!getWorkers.exec())
    {
        qDebug()<<"ERROR : unable to get workers from workers table";
    }
    else
    {
        if(flag==1)
        {
            qDebug() << "In FLAG 1";
            while(getWorkers.next())
            {
                QString text;
                text = getWorkers.value(0).toString();
                emit setComboBoxUsersMainW(text);
            }
        }
        else
            if(flag==2)
            {
                while(getWorkers.next())
                {
                    QString text;
                    text = getWorkers.value(0).toString();
                    qDebug() << "NEW USER " << text;
                    emit setComboBoxUsers(text);
                }
            }
        else if(flag==3)
            {
                while(getWorkers.next())
                {
                    QString text;
                    text = getWorkers.value(0).toString();
                    emit setComboBoxGetUserReport(text);
                }
            }
        isOkay = true;
    }
    return isOkay;
}
//Method used to open a connection to the database
bool MyDatabase::openConnection()
{
    QString path;
    path = "C:/Users/Petar/Desktop/Diplomska_Project_Juggernaut";
    qDebug()<<path;
    m_myDatabase=QSqlDatabase::addDatabase("QSQLITE");
    m_myDatabase.setDatabaseName(path+"/time_logger_db.sqlite");
    qDebug()<<"Open!";
    if(!m_myDatabase.open())
    {
        qDebug()<<"ERROR : "<< m_myDatabase.lastError().text();
        return false;
    }
    else
    {
        return true;
    }
}
//closing the connection we have with the database
void MyDatabase::closeConnection()
{
    m_myDatabase.removeDatabase(QSqlDatabase::defaultConnection);
    m_myDatabase.close();
    qDebug()<<"Close!";
}
void MyDatabase::setCurrentUser(QString user)
{
    m_currentUser = user;
}

QString MyDatabase::getCurrentUser()
{
    return m_currentUser;
}

int MyDatabase::getAction() const
{
    return m_action;
}

void MyDatabase::setAction(int value)
{
    m_action = value;
}

bool MyDatabase::createTablesQuery()
{
    bool isOkay = true;
    //This query is used to create the database tables, if they are already created then this part is ignored
    QSqlQuery queryCreateTables(m_myDatabase);
    queryCreateTables.prepare("CREATE TABLE IF NOT EXISTS workers (workeruser varchar(20) primary key not null unique,name varchar(20),surname varchar(20),password varchar(50), isadmin integer(1));");
    if(queryCreateTables.exec())
    {
        qDebug()<<"Table workers is available!";
    }
    else {
        isOkay = false;
    }
    queryCreateTables.clear();
    queryCreateTables.prepare("CREATE TABLE IF NOT EXISTS projects (projectname varchar(20) primary key not null unique,projectdescription varchar(200),startdate date, enddate date,companyname varchar(50),clientname varchar(30), projectworker varchar(20));");
    if(queryCreateTables.exec())
    {
        qDebug()<<"Table projects is available!";
    }
    else {
        isOkay = false;
    }
    queryCreateTables.clear();
    queryCreateTables.prepare("CREATE TABLE IF NOT EXISTS reports (wuser varchar(20) references workers(workeruser),pname varchar(20) references projects(projectname),spendtime int4, description varchar(200),reportdate date,constraint pkreport primary key (wuser,pname,reportdate));");
    if(queryCreateTables.exec())
    {
        qDebug()<<"Table reports is available!";
    }
    else {
        isOkay = false;
    }
    return isOkay;
}

bool MyDatabase::signUpCheckQuery(QString workeruser, QString name, QString surname, QString password, bool isAdmin)
{
    bool isOkay = false;

    QSqlQuery queryAvailable(m_myDatabase);
    queryAvailable.prepare("select workeruser from workers;");

    QByteArray cryptedPassword;
    cryptedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);

    if(!queryAvailable.exec())
    {
        qDebug()<<"ERROR : unable to get name of worker from workers table";
        isOkay = false;
    }
    else
    {
        bool checker = false;
        while(queryAvailable.next())
        {
            if(workeruser == queryAvailable.value(0))
            {
                qDebug()<<"Username already exists!";
                checker = true;
                break;
            }
        }
        //If the username is not taken then queryFillInTable is executed and the user is created in the database
        if(!checker)
        {
            QSqlQuery queryFillInTable(m_myDatabase);
            bool adminChecked = isAdmin ? true : false;
            queryFillInTable.prepare("insert into workers values ('" + workeruser + "','" + name + "','" + surname + "','" + cryptedPassword + "',:adminChecked);");
            queryFillInTable.bindValue(":adminChecked", adminChecked);
            if(queryFillInTable.exec())
            {
                qDebug()<<"Successfuly signed up!";
                isOkay = true;
            }
        }
    }
    return isOkay;
}

bool MyDatabase::logInCheckQuery(QString user, QByteArray cryptedPassword)
{
    bool isOkay = false;
    //Query that gets the login information out of the database for comparison with the input
    QSqlQuery queryAuthentication(m_myDatabase);
    queryAuthentication.prepare("select workeruser,password,isadmin from workers;");
    if(!queryAuthentication.exec())
    {
        qDebug()<<"ERROR : unable to get login information from workers table";
    }
    else
    {
        //This loop compares every password from the database with the input password untill it finds a match or it reaches end of database
        while(queryAuthentication.next())
        {
            if((queryAuthentication.value(0) == user) && (queryAuthentication.value(1) == cryptedPassword))
            {
                //if we find a match we show the time status window and update the data shown there to be the most recent one

                MyDatabase::instance()->setCurrentUser(user);
                MyDatabase::instance()->setIsAdmin(queryAuthentication.value(2).toBool());
                qDebug()<<"Login Succesfull! " << m_isAdmin;
                isOkay = true;
                break;
            }
            else {
                qDebug()<<"Did not match!";
            }
        }
    }
    return isOkay;
}

bool MyDatabase::setDonutChartQuery(QDate firstDayOfWeek, QDate lastDayOfWeek)
{
    bool isOkay = false;
    //Query that selects the total time spent for each project for the current week
    QSqlQuery getValues(m_myDatabase);
    getValues.prepare("select pname,sum(spendtime) from reports where wuser=='"+MyDatabase::instance()->getCurrentUser()+"'and reportdate BETWEEN date('"+firstDayOfWeek.toString("yyyy-MM-dd")+"') and date('"+lastDayOfWeek.toString("yyyy-MM-dd")+"') group by pname");
    if(!getValues.exec()){
        qDebug()<<"ERROR : Setting the donut chart failed!";
    }
    else
    {
        int counter = 0;
        //Query that sums up the total time the user has spent this week on the projects
        QSqlQuery getSum(m_myDatabase);
        getSum.prepare("Select sum(spendtime) from reports where wuser=='"+MyDatabase::instance()->getCurrentUser()+"'and reportdate BETWEEN date('"+firstDayOfWeek.toString("yyyy-MM-dd")+"') and date('"+lastDayOfWeek.toString("yyyy-MM-dd")+"')");
        if(!getSum.exec())
        {
            qDebug()<<"ERROR : Setting donut chart failed!";
        }
        else
        {
            getSum.first();
            counter=getSum.value(0).toInt();
            //Show the spent time this week
            if(MyDatabase::instance()->getIsAdmin() && !MyDatabase::instance()->getIsUser())
            {
                emit updateHoursAdmin(getSum.value(0).toString() == 0 ? "0" : getSum.value(0).toString());
            }
            else
                emit updateHours(getSum.value(0).toString() == 0 ? "0" : getSum.value(0).toString());

            isOkay = true;
        }
        //Loop that calculates the values and sets them to a clise in the pie
        while(getValues.next())
        {
            double br=(getValues.value(1).toDouble()/counter)*100;
            //The string show is the info that is labeled to each slice
            QString show;
            show.append(getValues.value(0).toString()+"\n");
            show.append(getValues.value(1).toString()+" hrs");
            show.append("["+QString::number(int(br))+"%]");

            setPieValues(show, getValues.value(1).toInt());
        }
        if(MyDatabase::instance()->getIsAdmin() && !MyDatabase::instance()->getIsUser())
        {
        }

    }
    return isOkay;
}

bool MyDatabase::setBarChartQuery(QDate firstDayOfWeek, QDate lastDayOfWeek)
{
    bool isOkay = false;
    //Query that selects the time spent on a project each day for a worker
    QSqlQuery loadHours(m_myDatabase);
    loadHours.prepare("Select spendtime,reportdate from reports where wuser=='"+MyDatabase::instance()->getCurrentUser()+"'and reportdate BETWEEN date('"+firstDayOfWeek.toString("yyyy-MM-dd")+"') and date('"+lastDayOfWeek.toString("yyyy-MM-dd")+"')");
    if(!loadHours.exec())
    {
        qDebug()<<"ERROR : Setting bar chart failed!";
    }
    else
    {
        int monday = 0;
        int tuesday = 0;
        int wednesday = 0;
        int thursday = 0;
        int friday = 0;
        int saturday = 0;
        int sunday = 0;
        //Loop that counts the hours spent each day for all the projects and taks
        while(loadHours.next())
        {
            QDate firstDate;
            firstDate = firstDayOfWeek;
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
        if(MyDatabase::instance()->getIsAdmin() && !MyDatabase::instance()->getIsUser())
        {
            emit setDaysAdmin(monday, tuesday, wednesday, thursday, friday, saturday, sunday);
        }
        else
            emit setDays(monday, tuesday, wednesday, thursday, friday, saturday, sunday);

        isOkay = true;
    }
    return isOkay;
}

bool MyDatabase::getProjectCountQuery()
{
    bool isOkay = false;
    QSqlQuery getSize(m_myDatabase);
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
    QSqlQuery getSize(m_myDatabase);
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

bool MyDatabase::getProjectQuery(int flag)
{
    bool isOkay=false;
    //Query for getting the name of the project so it can be set to the table
    QSqlQuery getProjects(m_myDatabase);
    getProjects.prepare("select projectname from projects where projectworker == '"+MyDatabase::instance()->getCurrentUser()+"'");
    if(!getProjects.exec())
    {
        qDebug()<<"ERROR : unable to get projects from projects table";
    }
    else
    {
        if(flag == 1){
            int i = 0;
            while(getProjects.next())
            {
                QString text;
                text = getProjects.value(0).toString();
                if(MyDatabase::instance()->getIsAdmin() && !MyDatabase::instance()->getIsUser())
                {
                    emit setProjectsAdmin(text,i);
                }
                else
                    emit setProjects(text,i);

                i++;
            }
        }
        if(flag == 2)
        {
            while(getProjects.next())
            {
                QString text;
                text = getProjects.value(0).toString();
                emit setCheckBox(text);
            }
        }
        if(flag == 3)
        {
            int counter = 0;
            while(getProjects.next())
            {
                QString text;
                text = getProjects.value(0).toString();
                emit setCheckBox(text);
                counter++;
            }
        }
        if(flag == 4)
        {
            while(getProjects.next())
            {
                QString text;
                text = getProjects.value(0).toString();
                emit setCheckBoxProjects(text);
            }
        }
        isOkay = true;
    }
    return isOkay;
}

bool MyDatabase::getReportsQuery(QDate firstDayOfWeek, QDate lastDayOfWeek)
{
    qDebug() << Q_FUNC_INFO << firstDayOfWeek << "|||" << lastDayOfWeek;
    bool isOkay = false;
    //Query that selects all the information from the report table that is needed to be displayed in the table
    QSqlQuery loadReport(m_myDatabase);
    loadReport.prepare("select reportdate,pname,description,spendtime from reports where wuser=='"+MyDatabase::instance()->getCurrentUser()+"'and reportdate BETWEEN date('"+firstDayOfWeek.toString("yyyy-MM-dd")+"') and date('"+lastDayOfWeek.toString("yyyy-MM-dd")+"')");
    if(!loadReport.exec())
    {
        qDebug()<<"ERROR : unable to get reports from reports table";
    }
    else
    {
        //Loop that fills in the table with the information from the query
        while(loadReport.next())
        {
                if(MyDatabase::instance()->getIsAdmin() && !MyDatabase::instance()->getIsUser())
                {
                    emit setReportsAdmin(1,1,loadReport.value(1).toString());
                }
                else {
                    QString reportDate = loadReport.value("reportdate").toString();
                    QString projectName = loadReport.value("pname").toString();
                    QString desc = loadReport.value("description").toString();
                    QString spendTime = loadReport.value("spendtime").toString();

                    emit setReports(reportDate, projectName, desc, spendTime);
                }

            emit addButtons(1);
            emit addToolTip(1);
        }
        isOkay = true;
    }
    return isOkay;
}

bool MyDatabase::getHoursQuery(QString projectName, QDate firstDayOfWeek, QDate lastDayOfWeek, int j)
{
    qDebug() << "FirstDay " << firstDayOfWeek << " LastDay "<<lastDayOfWeek;
    bool isOkay = false;
    QSqlQuery returnHours(m_myDatabase);
    returnHours.prepare("select reportdate,sum(spendtime) from reports where  pname=='"+projectName+"' and wuser=='"+MyDatabase::instance()->getCurrentUser()+"'and reportdate BETWEEN date('"+firstDayOfWeek.toString("yyyy-MM-dd")+"') and date('"+lastDayOfWeek.toString("yyyy-MM-dd")+"') group by reportdate ");
    if(!returnHours.exec())
    {
        qDebug()<<"ERROR : unable to return hours from reports table";
    }
    else
    {
        while(returnHours.next())
        {
            if(MyDatabase::instance()->getIsAdmin() && !MyDatabase::instance()->getIsUser())
            {
                emit setHoursAdmin(returnHours.value(0).toString(),returnHours.value(1).toString(),j);
            }
            else
                emit setHours(returnHours.value(0).toDate().dayOfWeek(),returnHours.value(1).toInt(),j);

        }
        isOkay = true;
    }
    return isOkay;
}

bool MyDatabase::deleteRowReportsQuery(QString user, QString project, QString date)
{
    bool isOkay = false;
    //Query used to insert the values in the table
    QSqlQuery queryDeleteRow(m_myDatabase);
    queryDeleteRow.prepare("DELETE FROM reports WHERE wuser='"+user+"'and pname='"+project+"'and reportdate='"+date+"';");
    if(queryDeleteRow.exec())
    {
        if(MyDatabase::instance()->getIsAdmin() && !MyDatabase::instance()->getIsUser())
        {
            emit dataUpdatedAdmin();
        }
        else
            emit dataUpdated();

        isOkay = true;
    }
    else
    {
        qDebug()<<"ERROR : deleting the row failed";
    }
    return isOkay;
}

bool MyDatabase::projectNameCheckQuery(QString projectName)
{
    bool isOkay = true;
    QSqlQuery queryAvailable(m_myDatabase);
    queryAvailable.prepare("select projectname from projects;");
    if(!queryAvailable.exec())
    {
        qDebug()<<"ERROR : unable to get projects from projects table";
    }
    else
    {
        //this checks if there is already a project with this name in the database, hinting that this project is created and already worked on
        while(queryAvailable.next())
        {
            if(projectName == queryAvailable.value(0))
            {
                qDebug()<<"This project already exists...";
                isOkay = false;
                break;
            }
        }
    }
    return isOkay;
}

bool MyDatabase::createProjectsRowQuery(QString projectName, QString projectDescription, QString startDate, QString endDate, QString companyName, QString clientName, QString projectWorker)
{
    bool isOkay = false;
    QSqlQuery queryFillInTable(m_myDatabase);
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

bool MyDatabase::createReportsRowQuery(const QString &person, const QString &project, const QString &time, const QString &description, const QString &date)
{
    QSqlQuery queryFillInTable(m_myDatabase);
    queryFillInTable.prepare("insert into reports values ('"+person+"','"+project+"','"+time+"','"+description+"','"+date+"');");

    if(queryFillInTable.exec()) {
        emit dataUpdated();
        return true;
    } else {
        qDebug()<<"ERROR : inserting row into reports failed";
        return false;
    }
}

bool MyDatabase::updateReportsRowQuery(const QString &person, const QString &project, const QString &time, const QString &description, const QString &date)
{
    qDebug() << person << date;

    QSqlQuery queryFillInTable(m_myDatabase);
    queryFillInTable.prepare("UPDATE reports SET(spendtime, description)=('" + time + "', '" + description + "')WHERE wuser='" + person + "' and pname='" + project + "'and reportdate='" + date + "';");

    if(queryFillInTable.exec())
        return true;
    else {
        qDebug()<<"ERROR : updating row in reports failed";
        return false;
    }
}

int MyDatabase::getAllReportsCount() const
{
    return m_allReportsCount;
}

bool MyDatabase::getAllUserReportsQuery(QString user, QString projectName, QString startDate, QString endDate)
{
    bool isOkay = false;
    //Query that selects all the information from the report table that is needed to be displayed in the table
    QSqlQuery loadReport(m_myDatabase);
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
    QSqlQuery getSize(m_myDatabase);
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

bool MyDatabase::getAllUserProjects(QString user)
{
    bool isOkay=false;
    //Query for getting the name of the project so it can be set to the table
    QSqlQuery getProjects(m_myDatabase);
    getProjects.prepare("select projectname from projects where projectworker == '"+user+"'");
    if(!getProjects.exec())
    {
        qDebug()<<"ERROR : unable to get all user projects from projects table";
    }
    else
    {
        while(getProjects.next())
        {
            QString text;
            text = getProjects.value(0).toString();
            emit setCheckBoxProjects(text);
        }
        isOkay = true;
    }
    return isOkay;
}

bool MyDatabase::getWorkerProjects(QString workerName)
{
    bool isOkay = false;
    QSqlQuery queryGetWorkerProjects(m_myDatabase);
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
