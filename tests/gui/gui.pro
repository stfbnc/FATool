TARGET = GUITest

CONFIG += qt warn_on depend_includepath testcase

APPDIR = ../../app
INCLUDEPATH += $$APPDIR

unix:!macx {
    INCLUDEPATH += $$APPDIR/includes
    LIBS += -L$$APPDIR/libs/ -lgsl -lgslcblas -lm
}

macx: {
    INCLUDEPATH += /opt/local/include/
    LIBS += -L/opt/local/lib/ -lgsl -lgslcblas -lm
}

QT = core testlib
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

SOURCES +=  \
    $$APPDIR/3rdparty_src/qcustomplot.cpp \
    $$APPDIR/log_window.cpp \
    $$APPDIR/mass_exponents_window.cpp \
    $$APPDIR/plot_window.cpp \
    $$APPDIR/main_window.cpp \
    $$APPDIR/file_ops.cpp \
    $$APPDIR/base_plot.cpp \
    $$APPDIR/inputs_dfa.cpp \
    $$APPDIR/inputs_dcca.cpp \
    $$APPDIR/inputs_ht.cpp \
    $$APPDIR/inputs_mfdfa.cpp \
    $$APPDIR/inputs_rhodcca.cpp \
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
    $$APPDIR/abstract_inputs_window.cpp \
    $$APPDIR/load_files_window.cpp \
    $$APPDIR/data_file.cpp \
    $$APPDIR/data_plot_window.cpp \
    $$APPDIR/files_data.cpp \
    $$APPDIR/update_table_widget.cpp \
    $$APPDIR/results_window.cpp \
    gui_test.cpp

HEADERS += \
    $$APPDIR/3rdparty_src/qcustomplot.h \
    $$APPDIR/constants.h \
    $$APPDIR/log_window.h \
    $$APPDIR/mass_exponents_window.h \
    $$APPDIR/plot_window.h \
    $$APPDIR/main_window.h \
    $$APPDIR/file_ops.h \
    $$APPDIR/base_plot.h \
    $$APPDIR/inputs_dfa.h \
    $$APPDIR/inputs_dcca.h \
    $$APPDIR/inputs_ht.h \
    $$APPDIR/inputs_mfdfa.h \
    $$APPDIR/inputs_rhodcca.h \
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
    $$APPDIR/legend_position_window.h \
    $$APPDIR/abstract_inputs_window.h \
    $$APPDIR/load_files_window.h \
    $$APPDIR/data_file.h \
    $$APPDIR/data_plot_window.h \
    $$APPDIR/files_data.h \
    $$APPDIR/update_table_widget.h \
    $$APPDIR/results_window.h

FORMS += \
    $$APPDIR/abstract_inputs_window.ui \
    $$APPDIR/load_files.ui \
    $$APPDIR/main_window.ui \
    $$APPDIR/plot_window.ui \
    $$APPDIR/move_legend_window.ui \
    $$APPDIR/refit_dialog.ui \
    $$APPDIR/fit_log.ui

RESOURCES += \
    test_data.qrc

release:DESTDIR = release
release:OBJECTS_DIR = release/.obj
release:MOC_DIR = release/.moc
release:RCC_DIR = release/.rcc
release:UI_DIR = release/.ui
