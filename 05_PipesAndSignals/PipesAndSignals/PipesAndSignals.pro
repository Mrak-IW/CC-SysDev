TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.c \
    src/firstProcessFunction.c \
    src/m1/firstProcessFunction.c \
    src/m2/secondProcessFunction.c

HEADERS += \
    include/processFunctions.h \
    src/m2/Include/processFunctions.h \
    src/m1/include/processFunctions.h \
    src/m2/include/processFunctions.h

