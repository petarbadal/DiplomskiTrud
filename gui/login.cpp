#include "login.h"

Login::Login(QObject *parent) :
    QObject(parent)
{
    openDBConnection();

    m_settings = new QSettings();
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

void Login::openDBConnection()
{
    //opens a connection to the database, if it can't connect it writes ERROR
    if(!MyDatabase::instance()->openConnection())
    {
        qDebug()<<"ERROR : connection with the database failed";
    }
    else
    {
        if(!MyDatabase::instance()->createTablesQuery())
        {
            qDebug()<<"ERROR : initialization of tables failed";
        }
        else{
            qDebug()<<"Succesfully initiated tables!";
        }
    }
}

bool Login::buttonLogInClicked(const QString user, const QString password)
{
    qDebug() << Q_FUNC_INFO << user << password;

    //Encripting the password so it can be compared with the one written in the database
    QByteArray cryptedPassword;
    cryptedPassword = QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Md5);

    if(!MyDatabase::instance()->logInCheckQuery(user,cryptedPassword))
    {
        qDebug() << "ERROR : Login information did not match";
        return false;
    } else {
        MyDatabase::instance()->setIsUser(1);

        return true;
    }
}

bool Login::buttonNextClicked(const QString userName) {
    if(!MyDatabase::instance()->checkUserNameQuery(userName))
        return false;

    return true;
}
