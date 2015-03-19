
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
    ../GDPLib/src/diverse.c \
    ../GDPLib/src/modell.c \
    ../GDPLib/src/konkurranse.c \
    ../GDPLib/src/kontroller.c \
    ../GDPLib/src/par.c \
    ../GDPLib/src/person.c \
    ../GDPLib/src/test.c \
    ../GDPLib/src/test_konkurranse.c \
    ../GDPLib/src/test_kontroller.c \
    ../GDPLib/src/test_par.c \
    ../GDPLib/src/test_modell.c \
    src/fil_versjon/gdpfilversjon.cpp


HEADERS += \
    ../GDPLib/src/gdpl.h \
    ../GDPLib/src/diverse.h \
    ../GDPLib/src/modell.h \
    ../GDPLib/src/person.h \
    ../GDPLib/src/par.h \
    ../GDPLib/src/kontroller.h \
    ../GDPLib/src/konkurranse.h \
    ../GDPLib/src/test_modell.h \
    src/fil_versjon/gdpfilversjon.h

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
