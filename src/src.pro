QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    build/debug/moc_changer.cpp \
#    build/debug/moc_illuminantplacechooser.cpp \
#    build/debug/moc_mainwindow.cpp \
#    build/debug/moc_moving.cpp \
    addlight.cpp \
    createscene.cpp \
    facade.cpp \
    main.cpp \
    mainwindow.cpp \
    mathelems.cpp \
    objectchange.cpp \
    objectdelete.cpp \
    objectmove.cpp \
    objects.cpp \
    placeobjects.cpp \
    specialgraphicsview.cpp

HEADERS += \
#    build/debug/moc_predefs.h \
    addlight.hpp \
    config.hpp \
    createscene.hpp \
    facade.hpp \
    mainwindow.h \
    mathelems.hpp \
    objectchange.hpp \
    objectdelete.hpp \
    objectmove.hpp \
    objects.hpp \
    placeobjects.h \
    specialgraphicsview.hpp

FORMS += \
    addarc.ui \
    addbrick.ui \
    addcylinder.ui \
    addlight.ui \
    addplate.ui \
    addtile.ui \
    createscene.ui \
    mainwindow.ui \
#    placecarсhooser.ui \
#    placecarсhooser.ui \
#    placeroadсhooser.ui \
#    placeroadсhooser.ui \
    objectchange.ui \
    objectdelete.ui \
    objectmove.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    proj.pro

DISTFILES += \
    build/.qmake.stash \
    build/object_script.src.Debug \
    build/object_script.src.Release \
    compare.py
