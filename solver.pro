TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIBS += -lboost_program_options

SOURCES += \
    top-factory.cpp \
    solver-main.cpp \
    solver.cpp \
    result.cpp \
    random-factory.cpp \
    pile-up.cpp \
    node.cpp \
    newtable.cpp \
    mutation-cache.cpp \
    math-stack.cpp \
    append-factory.cpp

HEADERS += \
    types.h \
    top-factory.h \
    timer.h \
    solver.h \
    result.h \
    random-factory.h \
    random.h \
    program-factory.h \
    pile-up.h \
    node.h \
    newtable.h \
    mutation-cache.h \
    math-stack.h \
    ivm.h \
    append-factory.h

