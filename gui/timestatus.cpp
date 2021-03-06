#include "timestatus.h"

TimeStatus::TimeStatus(QObject *parent) : QObject(parent) {
    connect(MyDatabase::instance(), SIGNAL(setReports(QString, QString, QString, QString)), this, SIGNAL(newReportReceived(QString, QString, QString, QString)));
    connect(MyDatabase::instance(), SIGNAL(setProjects(QString, int)), this, SIGNAL(newProjectReceived(QString, int)));
    connect(MyDatabase::instance(), SIGNAL(setHours(int, int, int)), this, SIGNAL(newProjectHoursReceived(int, int, int)));
    connect(MyDatabase::instance(), SIGNAL(updateHours(QString)), this, SLOT(onUpdatedHours(QString )));
    connect(MyDatabase::instance(), SIGNAL(setPieValues(QString, int)), this, SIGNAL(newPieValueReceived(QString, int)));
    connect(MyDatabase::instance(), SIGNAL(setDays(int, int, int, int, int, int, int)), this, SIGNAL(newBarValueReceived(int, int, int, int, int, int, int)));
    connect(MyDatabase::instance(), SIGNAL(updateIsAdmin(bool)), this, SLOT(setIsAdmin(bool)));
    connect(MyDatabase::instance(), SIGNAL(setComboBoxUsers(QString)), this, SIGNAL(newComboBoxUserReceived(QString)));
    connect(MyDatabase::instance(), SIGNAL(dataUpdated()), this, SLOT(onDataUpdated()));

    QDate currentDate(QDate::currentDate());
    m_firstDayOfWeek = currentDate.addDays(-(currentDate.dayOfWeek()) + 1);
    m_lastDayOfWeek = m_firstDayOfWeek.addDays(6);
}

void TimeStatus::setupGuiData() {
    setCurrentUser(MyDatabase::instance()->getCurrentUser());
    MyDatabase::instance()->getReportsQuery(m_firstDayOfWeek, m_lastDayOfWeek);
    MyDatabase::instance()->getProjectQuery();

    setDonutChart();
    setBarChart();

    setIsAdmin(MyDatabase::instance()->getIsAdmin());
}

void TimeStatus::setupProjectsHours(const QString &projectName, const int row) {
    MyDatabase::instance()->getHoursQuery(projectName, m_firstDayOfWeek, m_lastDayOfWeek, row);
}

void TimeStatus::setCurrentUser(const QString &name) {
    m_currentUser = name;
    MyDatabase::instance()->setCurrentUser(name);

    emit currentUserChanged(name);
}

void TimeStatus::buttonNextWeekClicked() {
    m_firstDayOfWeek = m_firstDayOfWeek.addDays(7);
    m_lastDayOfWeek = m_lastDayOfWeek.addDays(7);

    emit firstDayChanged(m_firstDayOfWeek.toString("MMMM dd"));
    emit lastDayChanged(m_lastDayOfWeek.toString("MMMM dd"));
    emit yearChanged(m_firstDayOfWeek.toString("yyyy"));
    emit clearAllData();

    onDataUpdated();
}

void TimeStatus::buttonPreviousWeekClicked() {
    m_firstDayOfWeek = m_firstDayOfWeek.addDays(-7);
    m_lastDayOfWeek = m_lastDayOfWeek.addDays(-7);

    emit firstDayChanged(m_firstDayOfWeek.toString("MMMM dd"));
    emit lastDayChanged(m_lastDayOfWeek.toString("MMMM dd"));
    emit yearChanged(m_firstDayOfWeek.toString("yyyy"));
    emit clearAllData();

    onDataUpdated();
}

void TimeStatus::onDataUpdated() {
    emit clearAllData();

    setDonutChart();
    setBarChart();
    MyDatabase::instance()->getReportsQuery(m_firstDayOfWeek, m_lastDayOfWeek);
    MyDatabase::instance()->getProjectQuery();
}

void TimeStatus::onUpdatedHours(const QString &value) {
    m_weeklyHoursSum = value;
    emit hoursSumChanged(m_weeklyHoursSum);
}

void TimeStatus::setDonutChart() {
    MyDatabase::instance()->setDonutChartQuery(m_firstDayOfWeek, m_lastDayOfWeek);
}

void TimeStatus::setBarChart() {
    MyDatabase::instance()->setBarChartQuery(m_firstDayOfWeek, m_lastDayOfWeek);
}

void TimeStatus::onButtonDeleteClicked(const QString &project, const QString &dateString) {
    MyDatabase::instance()->deleteRowReportsQuery(m_currentUser, project, dateString);
}

bool TimeStatus::onButtonSingUpClicked(const QString &user, const QString &name, const QString &surname, const QString &password, const bool signupAsAdmin) {
    if(!MyDatabase::instance()->signUpCheckQuery(user, password, name, surname, signupAsAdmin)) {
        qDebug()<<"Sign up failed";
        return false;
    }

    qDebug() << "Sign up is Successful";
    return true;
}

void TimeStatus::getUsersForComboBox() {
    emit clearAllUsersComboBoxData();

    if(!MyDatabase::instance()->getUsersQuery())
        qDebug() << "Fail to setup Workers ComboBox";
}

void TimeStatus::setIsAdmin(const bool isAdmin) {
    m_isAdmin = isAdmin;

    emit isAdminChanged(m_isAdmin);
}
