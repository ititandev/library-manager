#-------------------------------------------------
#
# Project created by QtCreator 2017-05-18T01:20:01
#
#-------------------------------------------------

QT       += core gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LibPro
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    loginform.cpp \
    readerform.cpp \
    usermanagerform.cpp \
    librarianform.cpp \
    userform.cpp \
    bookform.cpp \
    notiform.cpp \
    returnbookform.cpp

HEADERS  += mainwindow.h \
    loginform.h \
    readerform.h \
    usermanagerform.h \
    librarianform.h \
    userform.h \
    bookform.h \
    notiform.h \
    macro.h \
    returnbookform.h

FORMS    += \
    loginform.ui \
    readerform.ui \
    usermanagerform.ui \
    librarianform.ui \
    userform.ui \
    bookform.ui \
    notiform.ui \
    returnbookform.ui

RESOURCES += \
    res.qrc

RC_ICONS = libpro.ico

QMAKE_CXXFLAGS += -std=gnu++14
