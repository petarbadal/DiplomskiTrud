#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "gui/timestatus.h"
#include "gui/login.h"
#include "gui/addtimedialog.h"
#include "gui/assignproject.h"
#include "gui/getuserreport.h"

class AppCore : public QObject
{
    Q_OBJECT
public:
    explicit AppCore(QObject *parent = nullptr);
    void createAndLoadUi();
    void setupUiContext();
     QQmlContext *qmlContext() const { return engine_.rootContext(); }

private:
     QQmlApplicationEngine engine_;
};

#endif // APPCORE_H
