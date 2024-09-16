#QT       += core gui network
#
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#
#CONFIG += c++17
#
## You can make your code fail to compile if it uses deprecated APIs.
## In order to do so, uncomment the following line.
##DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#
#SOURCES += \
#    main.cpp \
#    mainwindow.cpp
#
#HEADERS += \
#    mainwindow.h
#
#FORMS += \
#    mainwindow.ui
#
## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += thread
CONFIG += warn_on

DEFINES += QT_DEBUG

SOURCES += \
    gptapikeyreview.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    gptapikeyreview.h \
    mainwindow.h

FORMS += \
    gptapikeyreview.ui \
    mainwindow.ui

# 리소스 파일 추가 예시
# RESOURCES += \
#    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
