TEMPLATE = app
TARGET = FATool
ICON = logo.icns

unix:!macx {
    INCLUDEPATH += usr/include
    LIBS += -L/lib/x86_64-linux-gnu -lgsl -lgslcblas -lm
}

macx: {
    INCLUDEPATH += /opt/local/include/
    LIBS += -L/opt/local/lib/ -lgsl -lgslcblas -lm
}

QT = core gui
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

SOURCES +=  \
    abstract_inputs_window.cpp \
    data_file.cpp \
    data_plot_window.cpp \
    files_data.cpp \
    load_files_window.cpp \
    main.cpp \
    log_window.cpp \
    mass_exponents_window.cpp \
    plot_window.cpp \
    qcustomplot.cpp \
    main_window.cpp \
    file_ops.cpp \
    results_window.cpp \
    save_window.cpp \
    base_plot.cpp \
    inputs_dfa.cpp \
    inputs_dcca.cpp \
    inputs_ht.cpp \
    inputs_mfdfa.cpp \
    inputs_rhodcca.cpp \
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
    legend_position_window.cpp \
    update_table_widget.cpp

HEADERS += \
    abstract_inputs_window.h \
    constants.h \
    data_file.h \
    data_plot_window.h \
    files_data.h \
    load_files_window.h \
    log_window.h \
    mass_exponents_window.h \
    plot_window.h \
    qcustomplot.h \
    main_window.h \
    file_ops.h \
    results_window.h \
    save_window.h \
    base_plot.h \
    inputs_dfa.h \
    inputs_dcca.h \
    inputs_ht.h \
    inputs_mfdfa.h \
    inputs_rhodcca.h \
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
    legend_position_window.h \
    update_table_widget.h

#DISTFILES += \
#    prefs.txt

FORMS += \
    abstract_inputs_window.ui \
    fit_log.ui \
    load_files.ui \
    main_window.ui \
    move_legend_window.ui \
    plot_window.ui \
    refit_dialog.ui

RESOURCES += \
    starting_message.qrc
