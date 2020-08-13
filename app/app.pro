TEMPLATE = app
TARGET = FATool
ICON = logo.icns

unix:!macx {
    INCLUDEPATH += usr/include
#    INCLUDEPATH += /lib/gcc/x86_64-linux-gnu/9/include
#    LIBS += -L/lib/gcc/x86_64-linux-gnu/9 -lgomp
    LIBS += -L/lib/x86_64-linux-gnu -lgsl -lgslcblas -lm
#    QMAKE_CXXFLAGS += -fopenmp
#    LIBS += -fopenmp
}

macx: {
    INCLUDEPATH += /opt/local/include/
#    INCLUDEPATH += /opt/local/include/libomp/
#    LIBS += -L/opt/local/lib/libomp -lomp
    LIBS += -L/opt/local/lib/ -lgsl -lgslcblas -lm
#    QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp
#    LIBS += -Xpreprocessor -fopenmp
}

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

SOURCES +=  \
    abstract_inputs_window.cpp \
    main.cpp \
    log_window.cpp \
    mass_exponents_window.cpp \
    plot_window.cpp \
    qcustomplot.cpp \
    main_window.cpp \
    file_ops.cpp \
    save_window.cpp \
    base_plot.cpp \
    inputs_window.cpp \
    inputs_dfa.cpp \
    inputs_dcca.cpp \
    inputs_ht.cpp \
    inputs_mfdfa.cpp \
    inputs_rhodcca.cpp \
    dfa_window.cpp \
    mfdfa_window.cpp \
    dcca_window.cpp \
    rhodcca_window.cpp \
    ht_window.cpp \
    DFA.cpp \
    array_ops.cpp \
    math_ops.cpp \
    refit_window.cpp \
    DCCA.cpp \
    rhoDCCA.cpp \
    spectrum_window.cpp \
    starting_window.cpp \
    MFDFA.cpp \
    MFDFA_single_q.cpp \
    HT.cpp \
    HT_single_scale.cpp \
    legend_position_window.cpp

HEADERS += \
    abstract_inputs_window.h \
    constants.h \
    log_window.h \
    mass_exponents_window.h \
    plot_window.h \
    qcustomplot.h \
    main_window.h \
    file_ops.h \
    save_window.h \
    base_plot.h \
    inputs_window.h \
    inputs_dfa.h \
    inputs_dcca.h \
    inputs_ht.h \
    inputs_mfdfa.h \
    inputs_rhodcca.h \
    dfa_window.h \
    mfdfa_window.h \
    dcca_window.h \
    rhodcca_window.h \
    ht_window.h \
    DFA.h \
    array_ops.h \
    math_ops.h \
    FA.h \
    refit_window.h \
    DCCA.h \
    rhoDCCA.h \
    spectrum_window.h \
    starting_window.h \
    MFDFA.h \
    MFDFA_single_q.h \
    HT.h \
    HT_single_scale.h \
    legend_position_window.h

DISTFILES += \
    prefs.txt

FORMS += \
    abstract_inputs_window.ui
