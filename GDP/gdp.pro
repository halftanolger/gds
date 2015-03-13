
QT += widgets

CONFIG += qt \
     warn_on \
     link_prl \
     c++11 \
     verbose

QMAKE_CXXFLAGS += -std=c++11 -g

#--------------------------------------------------
# GDPLib
#
SOURCES += \
    ../GDPLib/src/konkurranse.c \
    ../GDPLib/src/kontroller.c \
    ../GDPLib/src/par.c \
    ../GDPLib/src/person.c \
    ../GDPLib/src/test.c \
    ../GDPLib/src/test_konkurranse.c \
    ../GDPLib/src/test_kontroller.c \
    ../GDPLib/src/test_par.c \
    ../GDPLib/src/test_person.c \

HEADERS += \
    ../GDPLib/src/gdpl.h \

DEPENDPATH += \
    ../GDPLib/src/ \

INCLUDEPATH += \
    ../GDPLib/src/ \
#
#
#--------------------------------------------------


SOURCES += \
    src/main.cpp \
    src/gdpmainwindows.cpp \

HEADERS += \
    src/gdpmainwindows.h \
