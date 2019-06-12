TEMPLATE = app
TARGET = FATool
ICON = logo.icns

LIBS += -L/opt/local/lib/libomp -lomp
QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp
LIBS += -Xpreprocessor -fopenmp

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
    MathOps.cpp \
    refit_window.cpp \
    DCCA.cpp \
    rhoDCCA.cpp \
    starting_window.cpp \
    MFDFA.cpp \
    MFDFAsingleQ.cpp \
    HT.cpp \
    HTsingleScale.cpp \
    legend_position_window.cpp

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
    MathOps.h \
    FA.h \
    refit_window.h \
    DCCA.h \
    rhoDCCA.h \
    starting_window.h \
    MFDFA.h \
    MFDFAsingleQ.h \
    HT.h \
    HTsingleScale.h \
    legend_position_window.h \
    omp.h

DISTFILES += \
    prefs.txt
