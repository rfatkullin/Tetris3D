#-------------------------------------------------
#
# Project created by QtCreator 2011-10-08T17:39:21
#
#-------------------------------------------------

QT       += core opengl phonon

TARGET = Tetris3D
CONFIG   += console opengl gui
CONFIG   -= app_bundle

TEMPLATE = app

RC_FILE = resource.rc

SOURCES += \
    Material.cpp \
    Game.cpp \
    main.cpp \
    Scene.cpp \
    GameObjects.cpp \
    Geometry.cpp \
    GeomEntity.cpp \
    PhisEntity.cpp \
    MainWindow.cpp \
    SelectFiguresDialog.cpp \
    GameOverDIalog.cpp \
    TopViewDialog.cpp \
    AboutDialog.cpp \
    ControlDialog.cpp

HEADERS += \
    Material.h \
    Game.h \
    Scene.h \
    GameObjects.h \
    Geometry.h \
    GeomEntity.h \
    PhisEntity.h \
    MainWindow.h \
    SelectFiguresDialog.h \
    GameOverDialog.h \
    TopViewDialog.h \
    AboutDialog.h \
    ControlDialog.h

OTHER_FILES += \
    save.dat \
    Help.html \
    resource.rc \
    Tetris3D.rar












































