#include <QApplication>

#include "core/appcore.h"
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QString("TeamMApplication"));
    app.setApplicationVersion(QString("1.0"));
    app.setWindowIcon(QIcon(":/images/images/logo.svg"));

    AppCore core;
    core.createAndLoadUi();

    return app.exec();
}

