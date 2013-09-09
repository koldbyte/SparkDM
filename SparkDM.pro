#-------------------------------------------------
#
# Project created by QtCreator 2013-06-27T10:33:16
#
#-------------------------------------------------

QT       += core gui
QT	 +=sql
QT	 +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SparkDM
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    backend/modeldownloads.cpp \
    backend/downloadsdbmanager.cpp \
    dialogs/optionsdialog.cpp \
    dialogs/newdownloadinfodialog.cpp \
    dialogs/downloadpropertydialog.cpp \
    dialogs/downloadingprogressdialog.cpp \
    dialogs/downloadfinisheddialog.cpp \
    dialogs/adddownloaddialog.cpp \
    systemtrayicon.cpp \
    backend/networkaccessmanager.cpp \
    backend/autosaver.cpp \
    backend/downloads.cpp

HEADERS  += mainwindow.h \
    backend/modeldownloads.h \
    backend/downloadsdbmanager.h \
    dialogs/optionsdialog.h \
    dialogs/newdownloadinfodialog.h \
    dialogs/downloadpropertydialog.h \
    dialogs/downloadingprogressdialog.h \
    dialogs/downloadfinisheddialog.h \
    dialogs/adddownloaddialog.h \
    systemtrayicon.h \
    backend/networkaccessmanager.h \
    backend/autosaver.h \
    backend/ui_proxy.h \
    backend/ui_passworddialog.h \
    backend/downloads.h

FORMS    += mainwindow.ui \
    dialogs/optionsdialog.ui \
    dialogs/newdownloadinfodialog.ui \
    dialogs/downloadpropertydialog.ui \
    dialogs/downloadingprogressdialog.ui \
    dialogs/downloadfinisheddialog.ui \
    dialogs/adddownloaddialog.ui \
    backend/proxy.ui \
    backend/passworddialog.ui

RESOURCES += images.qrc
