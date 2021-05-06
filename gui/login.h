#ifndef LOGIN_H
#define LOGIN_H

#include "database/mydatabase.h"

class Login : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString savedUserName  READ savedUserName WRITE setSavedUserName NOTIFY savedUserNameChanged)
    Q_PROPERTY(QString savedUserPass READ savedUserPass WRITE setSavedUserPass NOTIFY savedUserPassChanged)

    explicit Login(QObject *parent = nullptr);

    Q_INVOKABLE bool buttonLogInClicked(const QString &user, const QString &password);
    Q_INVOKABLE bool buttonNextClicked(const QString &userName);
    Q_INVOKABLE bool saveCreditentials(const QString &user,const QString &password);
    Q_INVOKABLE bool deleteCreditentials();

    QString savedUserName() const {
        return m_settings->value("user").toString();
    }

    QString savedUserPass() const {
        return m_settings->value("password").toString();
    }

signals:
    void savedUserNameChanged(QString name);
    void savedUserPassChanged(QString pass);

public slots:
    void setSavedUserName(QString name);
    void setSavedUserPass(QString pass);

private:
    void openDBConnection();

    QSettings *m_settings;
};

#endif // LOGIN_H
