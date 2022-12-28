#-------------------------------------------------
#
# Project created by QtCreator 2020-03-18T08:49:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = icvs_avvs
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
        src/main_window.cpp \
        src/map_edit_dlg.cpp \
        src/can_debug_dlg.cpp \
        src/map_select_dlg.cpp \
        src/settings.cpp \
        src/addtopics.cpp \
        src/task_manage_dlg.cpp 

HEADERS += \
        include/main_window.hpp \
        include/map_edit_dlg.h \
        include/can_debug_dlg.h \
        include/map_select_dlg.h \
        include/settings.h \
        include/addtopics.h \
        include/task_manage_dlg.h 

FORMS += \
        ui/main_window.ui \
        ui/addtopics.ui \
        ui/can_debug_dlg.ui \
        ui/map_edit_dlg.ui \
        ui/map_select_dlg.ui \
        ui/settings.ui \
        ui/task_manage_dlg.ui 

RESOURCES += \
    resources/images.qrc \
    resources/media.qrc 