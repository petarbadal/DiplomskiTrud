#include "appcore.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QDebug>

AppCore::AppCore(QObject *parent) : QObject(parent)
{
    qmlRegisterType<Login>("BackendAccess", 1, 0, "LoginCore");
    qmlRegisterType<TimeStatus>("BackendAccess", 1, 0, "TimeStatusCore");
    qmlRegisterType<AddTimeDialog>("BackendAccess", 1, 0, "AddTimeCore");
    qmlRegisterType<AssignProject>("BackendAccess", 1, 0, "AssignProjectCore");
    qmlRegisterType<GetUserReport>("BackendAccess", 1, 0, "GetUserReport");
}

void AppCore::createAndLoadUi()
{
    //qDebug() << Q_FUNC_INFO;

    //setupUiContext();

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    QObject::connect(
        &engine_, &QQmlApplicationEngine::objectCreated,
        QApplication::instance(),
        [url, this](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                QApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    engine_.load(url);
}

void AppCore::setupUiContext()
{
}
