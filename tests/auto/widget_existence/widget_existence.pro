TARGET = WidgetExistenceTest

CONFIG += qt warn_on depend_includepath testcase

APPDIR = ../../../app
INCLUDEPATH += $$APPDIR
INCLUDEPATH += /opt/local/include/
INCLUDEPATH += /opt/local/include/libomp/
LIBS += -L/opt/local/lib/libomp -lomp
LIBS += -L/opt/local/lib/ -lgsl -lgslcblas -lm
QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp
LIBS += -Xpreprocessor -fopenmp

QT = core testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

SOURCES +=  \
#    $$files($$APPDIR/*.cpp, true) \
    $$APPDIR/log_window.cpp \
    $$APPDIR/mass_exponents_window.cpp \
    $$APPDIR/plot_window.cpp \
    $$APPDIR/qcustomplot.cpp \
    $$APPDIR/main_window.cpp \
    $$APPDIR/file_ops.cpp \
    $$APPDIR/save_window.cpp \
    $$APPDIR/base_plot.cpp \
    $$APPDIR/inputs_window.cpp \
    $$APPDIR/inputs_dfa.cpp \
    $$APPDIR/inputs_dcca.cpp \
    $$APPDIR/inputs_ht.cpp \
    $$APPDIR/inputs_mfdfa.cpp \
    $$APPDIR/inputs_rhodcca.cpp \
    $$APPDIR/dfa_window.cpp \
    $$APPDIR/mfdfa_window.cpp \
    $$APPDIR/dcca_window.cpp \
    $$APPDIR/rhodcca_window.cpp \
    $$APPDIR/ht_window.cpp \
    $$APPDIR/DFA.cpp \
    $$APPDIR/array_ops.cpp \
    $$APPDIR/math_ops.cpp \
    $$APPDIR/refit_window.cpp \
    $$APPDIR/DCCA.cpp \
    $$APPDIR/rhoDCCA.cpp \
    $$APPDIR/spectrum_window.cpp \
    $$APPDIR/starting_window.cpp \
    $$APPDIR/MFDFA.cpp \
    $$APPDIR/MFDFA_single_q.cpp \
    $$APPDIR/HT.cpp \
    $$APPDIR/HT_single_scale.cpp \
    $$APPDIR/legend_position_window.cpp \
    TestWidgetExistence.cpp

HEADERS += \
#    $$files($$APPDIR/*.h, true) \
    $$APPDIR/constants.h \
    $$APPDIR/log_window.h \
    $$APPDIR/mass_exponents_window.h \
    $$APPDIR/plot_window.h \
    $$APPDIR/qcustomplot.h \
    $$APPDIR/main_window.h \
    $$APPDIR/file_ops.h \
    $$APPDIR/save_window.h \
    $$APPDIR/base_plot.h \
    $$APPDIR/inputs_window.h \
    $$APPDIR/inputs_dfa.h \
    $$APPDIR/inputs_dcca.h \
    $$APPDIR/inputs_ht.h \
    $$APPDIR/inputs_mfdfa.h \
    $$APPDIR/inputs_rhodcca.h \
    $$APPDIR/dfa_window.h \
    $$APPDIR/mfdfa_window.h \
    $$APPDIR/dcca_window.h \
    $$APPDIR/rhodcca_window.h \
    $$APPDIR/ht_window.h \
    $$APPDIR/DFA.h \
    $$APPDIR/array_ops.h \
    $$APPDIR/math_ops.h \
    $$APPDIR/FA.h \
    $$APPDIR/refit_window.h \
    $$APPDIR/DCCA.h \
    $$APPDIR/rhoDCCA.h \
    $$APPDIR/spectrum_window.h \
    $$APPDIR/starting_window.h \
    $$APPDIR/MFDFA.h \
    $$APPDIR/MFDFA_single_q.h \
    $$APPDIR/HT.h \
    $$APPDIR/HT_single_scale.h \
    $$APPDIR/legend_position_window.h

