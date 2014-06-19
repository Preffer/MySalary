#-------------------------------------------------
#
# Project created by QtCreator 2014-06-16T22:21:11
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MySalary
TEMPLATE = app


SOURCES += main.cpp\
        mysalary.cpp \
    paintedWidget.cpp

HEADERS  += mysalary.h

FORMS    += mysalary.ui

RESOURCES += \
    resource.qrc

RC_FILE = logo.rc
