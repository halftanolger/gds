
CONFIG +=  \
     warn_on \     
     c++11 \
     verbose \
     console \


QMAKE_CXXFLAGS += -std=c++11 -g

#--------------------------------------------------
# GDPLib
#
SOURCES += \
    ../GDPLib/src/modell.c \
    ../GDPLib/src/konkurranse.c \
    ../GDPLib/src/kontroller.c \
    ../GDPLib/src/par.c \
    ../GDPLib/src/person.c \
    ../GDPLib/src/log.c


HEADERS += \
    ../GDPLib/src/gdpl.h \
    ../GDPLib/src/modell.h \
    ../GDPLib/src/person.h \
    ../GDPLib/src/par.h \
    ../GDPLib/src/kontroller.h \
    ../GDPLib/src/konkurranse.h \
    ../GDPLib/src/log.h

DEPENDPATH += \
    ../GDPLib/src/ \

INCLUDEPATH += \
    ../GDPLib/src/ \
#
#
#--------------------------------------------------


SOURCES += \
    src/main.cpp \    

HEADERS += \

