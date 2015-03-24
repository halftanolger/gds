#
# QMake byggeskript
#


CONFIG +=  \
     warn_on \
     c++11 \
     verbose


QMAKE_CXXFLAGS += -std=c++11 -g

#--------------------------------------------------
# GDPLib
#
SOURCES += \
    src/modell.c \
    src/konkurranse.c \
    src/kontroller.c \
    src/par.c \
    src/person.c \
    src/log.c \


HEADERS += \
    src/gdpl.h \
    src/modell.h \
    src/person.h \
    src/par.h \
    src/kontroller.h \
    src/konkurranse.h \
    src/log.h \

DEPENDPATH += \
    src/ \

INCLUDEPATH += \
    src/ \
#
#
#--------------------------------------------------



#--------------------------------------------------
# GDPLib - testklient
#
SOURCES += \
    src/main.c \

HEADERS += \
    src/main.h \
#
#
#--------------------------------------------------
    
    
    
