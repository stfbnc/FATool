TEMPLATE = app
TARGET = FATool
ICON = logo.icns

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

SOURCES +=  main.cpp \
    plot_window.cpp \
    qcustomplot.cpp \
    main_window.cpp \
    FileOps.cpp \
    savewindow.cpp \
    base_plot.cpp \
    inputs_window.cpp \
    DFA.cpp \
    ArrayOps.cpp \
    MathOps.cpp

HEADERS += \
    plot_window.h \
    qcustomplot.h \
    main_window.h \
    FAGlobs.h \
    FileOps.h \
    savewindow.h \
    base_plot.h \
    inputs_window.h \
    DFA.h \
    ArrayOps.h \
    MathOps.h
