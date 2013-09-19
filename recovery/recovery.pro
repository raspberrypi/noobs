#-------------------------------------------------
#
# Project created by QtCreator 2013-04-30T12:10:31
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = recovery
TEMPLATE = app
LIBS += -lqjson

system(sh updateqm.sh 2>/dev/null)

SOURCES += main.cpp\
        mainwindow.cpp \
    languagedialog.cpp \
    initdrivethread.cpp \
    keydetection.cpp \
    gpioinput.cpp \
    progressslideshowdialog.cpp \
    confeditdialog.cpp \
    rightbuttonfilter.cpp \
    json.cpp \
    multiimagewritethread.cpp \
    util.cpp \
    twoiconsdelegate.cpp \
    bootselectiondialog.cpp

HEADERS  += mainwindow.h \
    languagedialog.h \
    initdrivethread.h \
    config.h \
    keydetection.h \
    gpioinput.h \
    mbr.h \
    progressslideshowdialog.h \
    confeditdialog.h \
    rightbuttonfilter.h \
    json.h \
    multiimagewritethread.h \
    util.h \
    twoiconsdelegate.h \
    bootselectiondialog.h

FORMS    += mainwindow.ui \
    languagedialog.ui \
    progressslideshowdialog.ui \
    confeditdialog.ui \
    bootselectiondialog.ui

RESOURCES += \
    icons.qrc

TRANSLATIONS += translation_nl.ts \
    translation_de.ts \
    translation_pt.ts \
    translation_ja.ts \
    translation_fr.ts \
    translation_hu.ts \
    translation_fi.ts

OTHER_FILES += \
    README.txt
