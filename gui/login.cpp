#include "login.h"

Login::Login(QObject *parent) :
    QObject(parent),
    m_settings(new QSettings)
{
    openDBConnection();
}

bool Login::saveCreditentials(const QString &user, const QString &password)
{
    m_settings->setValue("user", user);
    m_settings->setValue("password", password);

    emit savedUserNameChanged(user);
    emit savedUserPassChanged(password);

    return true;
}

bool Login::deleteCreditentials()
{
    m_settings->setValue("user", "");
    m_settings->setValue("password", "");

    return true;
}

void Login::setSavedUserName(QString name)
{
    m_settings->setValue("user", name);

    emit savedUserNameChanged(name);
}

void Login::setSavedUserPass(QString pass)
{
    m_settings->setValue("password", pass);

    emit savedUserPassChanged(pass);
}

void Login::openDBConnection() {
    MyDatabase::instance()->openConnection();
    MyDatabase::instance()->createTablesQuery();
}

bool Login::buttonLogInClicked(const QString &user, const QString &password) {
    QByteArray cryptedPassword(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5));

    return MyDatabase::instance()->logInCheckQuery(user, cryptedPassword);
}

bool Login::buttonNextClicked(const QString &userName) {
    return MyDatabase::instance()->checkUserNameQuery(userName);
}
