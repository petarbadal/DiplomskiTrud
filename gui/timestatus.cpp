#include "timestatus.h"

TimeStatus::TimeStatus(QObject *parent) :
    QObject(parent)
{
    connect(MyDatabase::instance(), SIGNAL(setReports(QString, QString, QString, QString)), this, SIGNAL(newReportReceived(QString, QString, QString, QString)));
    connect(MyDatabase::instance(), SIGNAL(setProjects(QString, int)), this, SIGNAL(newProjectReceived(QString, int)));
    connect(MyDatabase::instance(), SIGNAL(setHours(int, int, int)), this, SIGNAL(newProjectHoursReceived(int, int, int)));

    connect(MyDatabase::instance(), SIGNAL(updateHours(QString)), this, SLOT(onUpdatedHours(QString )));

    connect(MyDatabase::instance(), SIGNAL(setPieValues(QString, int)), this, SIGNAL(newPieValueReceived(QString, int)));

    connect(MyDatabase::instance(), SIGNAL(setDays(int, int, int, int, int, int, int)), this, SIGNAL(newBarValueReceived(int, int, int, int, int, int, int)));

    connect(MyDatabase::instance(), SIGNAL(updateIsAdmin(bool)), this, SLOT(setIsAdmin(bool)));

    connect(MyDatabase::instance(), SIGNAL(setComboBoxUsersMainW(QString)), this, SIGNAL(newComboBoxUserReceived(QString)));

    connect(MyDatabase::instance(), SIGNAL(dataUpdated()), this, SLOT(onDataUpdated()));

    QDate currentDate(QDate::currentDate());
    m_firstDayOfWeek = currentDate.addDays(-(currentDate.dayOfWeek()) + 1);
    m_lastDayOfWeek = m_firstDayOfWeek.addDays(6);
}

TimeStatus::~TimeStatus()
{}

void TimeStatus::setupGuiData()
{
    //qDebug() << Q_FUNC_INFO << MyDatabase::instance()->getCurrentUser();

    setCurrentUser(MyDatabase::instance()->getCurrentUser());
    MyDatabase::instance()->getReportsQuery(m_firstDayOfWeek,m_lastDayOfWeek);
    MyDatabase::instance()->getProjectQuery(1);

    setDonutChart();
    setBarChart();

    setIsAdmin(MyDatabase::instance()->getIsAdmin());
}

void TimeStatus::setupProjectsHours(const QString &projectName, const int row)
{
    MyDatabase::instance()->getHoursQuery(projectName, m_firstDayOfWeek, m_lastDayOfWeek, row);
}

void TimeStatus::setCurrentUser(const QString &name)
{
    //qDebug() << Q_FUNC_INFO << name;

    currentUser_ = name;
    MyDatabase::instance()->setCurrentUser(name);

    emit currentUserChanged(name);
}

void TimeStatus::buttonNextWeekClicked()
{
    m_firstDayOfWeek=m_firstDayOfWeek.addDays(7);
    m_lastDayOfWeek=m_lastDayOfWeek.addDays(7);

    emit firstDayChanged(m_firstDayOfWeek.toString("MMMM dd"));
    emit lastDayChanged(m_lastDayOfWeek.toString("MMMM dd"));
    emit yearChanged(m_firstDayOfWeek.toString("yyyy"));
    emit clearAllData();

    onDataUpdated();
}

void TimeStatus::buttonPreviousWeekClicked()
{
    m_firstDayOfWeek=m_firstDayOfWeek.addDays(-7);
    m_lastDayOfWeek=m_lastDayOfWeek.addDays(-7);

    emit firstDayChanged(m_firstDayOfWeek.toString("MMMM dd"));
    emit lastDayChanged(m_lastDayOfWeek.toString("MMMM dd"));
    emit yearChanged(m_firstDayOfWeek.toString("yyyy"));
    emit clearAllData();

    onDataUpdated();
}

void TimeStatus::onDataUpdated()
{
    emit clearAllData();

    setDonutChart();
    setBarChart();
    MyDatabase::instance()->getReportsQuery(m_firstDayOfWeek,m_lastDayOfWeek);
    MyDatabase::instance()->getProjectQuery(1);
}

void TimeStatus::onUpdatedHours(const QString &value)
{
    //qDebug() << "Hours this week: " << value ;

    weeklyHoursSum_ = value;
    emit hoursSumChanged(weeklyHoursSum_);
}

void TimeStatus::setDonutChart()
{
    MyDatabase::instance()->setDonutChartQuery(m_firstDayOfWeek,m_lastDayOfWeek);
}

void TimeStatus::setBarChart()
{
    MyDatabase::instance()->setBarChartQuery(m_firstDayOfWeek,m_lastDayOfWeek);
}

void TimeStatus::onButtonDeleteClicked(const QString &project, const QString &dateString)
{
    MyDatabase::instance()->deleteRowReportsQuery(currentUser_, project,dateString);
}

bool TimeStatus::onButtonSingUpClicked(const QString &user, const QString &name, const QString &surname, const QString &password, bool signupAsAdmin)
{
    //qDebug() << Q_FUNC_INFO << user << name << surname << password << signupAsAdmin;

    if(!MyDatabase::instance()->signUpCheckQuery(user, password, name, surname, signupAsAdmin))
    {
        qDebug()<<"Sign up failed";
        return false;
    }

    qDebug() << "Sign up is Successful";
    return true;
}

void TimeStatus::getUsersForComboBox()
{
    //qDebug() << Q_FUNC_INFO;

    emit clearAllUsersComboBoxData();

    MyDatabase::instance()->getUsersQuery(1);
}

void TimeStatus::setIsAdmin(bool isAdmin)
{
    //qDebug() << Q_FUNC_INFO << isAdmin;

    isAdmin_ = isAdmin;

    emit isAdminChanged(isAdmin_);
}
