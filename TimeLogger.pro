#-------------------------------------------------
#
# Project created by QtCreator 2020-012-09T22:14:01
#
#-------------------------------------------------

QT += core gui sql quick widgets

greaterThan(QT_MAJOR_VERSION, 4)

TARGET = TimeLogger
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CFLAGS += -std=c99

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    core/appcore.cpp \
    database/mydatabase.cpp \
    gui/addtimedialog.cpp \
    gui/assignproject.cpp \
    gui/getuserreport.cpp \
    gui/login.cpp \
    gui/timestatus.cpp \
        main.cpp \

HEADERS += \
    core/appcore.h \
    database/mydatabase.h \
    gui/addtimedialog.h \
    gui/assignproject.h \
    gui/getuserreport.h \
    gui/login.h \
    gui/timestatus.h \

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
