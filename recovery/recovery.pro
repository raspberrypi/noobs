#-------------------------------------------------
#
# Project created by QtCreator 2013-04-30T12:10:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = recovery
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    imagewritethread.cpp \
    languagedialog.cpp \
    initdrivethread.cpp \
    keydetection.cpp \
    gpioinput.cpp \
    progressslideshowdialog.cpp \
    confeditdialog.cpp \
    rightbuttonfilter.cpp

HEADERS  += mainwindow.h \
    imagewritethread.h \
    languagedialog.h \
    initdrivethread.h \
    config.h \
    keydetection.h \
    gpioinput.h \
    mbr.h \
    progressslideshowdialog.h \
    confeditdialog.h \
    rightbuttonfilter.h

FORMS    += mainwindow.ui \
    languagedialog.ui \
    progressslideshowdialog.ui \
    confeditdialog.ui

RESOURCES += \
    icons.qrc

TRANSLATIONS += translation_nl.ts \
    translation_de.ts \
    translation_pt.ts \
    translation_ja.ts \
    translation_fr.ts

OTHER_FILES += \
    README.txt
