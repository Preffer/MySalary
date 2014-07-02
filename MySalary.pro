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

CONFIG += mobility
MOBILITY =

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = /home/hyz/Desktop/MySalary/MySalary/../../../build2/build/libs/armeabi-v7a/libmariadb.so
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../build2/build/libs/armeabi-v7a/release/ -lmariadbclient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../build2/build/libs/armeabi-v7a/debug/ -lmariadbclient
else:unix: LIBS += -L$$PWD/../../../build2/build/libs/armeabi-v7a/ -lmariadbclient

INCLUDEPATH += $$PWD/../../../build2/build/libs/armeabi-v7a
DEPENDPATH += $$PWD/../../../build2/build/libs/armeabi-v7a

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../build2/build/libs/armeabi-v7a/release/libmariadbclient.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../build2/build/libs/armeabi-v7a/debug/libmariadbclient.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../build2/build/libs/armeabi-v7a/release/mariadbclient.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../build2/build/libs/armeabi-v7a/debug/mariadbclient.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../build2/build/libs/armeabi-v7a/libmariadbclient.a
