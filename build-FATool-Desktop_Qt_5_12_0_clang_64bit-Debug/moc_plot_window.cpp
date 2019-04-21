/****************************************************************************
** Meta object code from reading C++ file 'plot_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../plot_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plot_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PlotWindow_t {
    QByteArrayData data[10];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlotWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlotWindow_t qt_meta_stringdata_PlotWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "PlotWindow"
QT_MOC_LITERAL(1, 11, 12), // "onYorNLegend"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12), // "onRefitClick"
QT_MOC_LITERAL(4, 38, 13), // "onReplotClick"
QT_MOC_LITERAL(5, 52, 15), // "onSavePlotClick"
QT_MOC_LITERAL(6, 68, 14), // "onSaveTxtClick"
QT_MOC_LITERAL(7, 83, 6), // "newFit"
QT_MOC_LITERAL(8, 90, 5), // "start"
QT_MOC_LITERAL(9, 96, 3) // "end"

    },
    "PlotWindow\0onYorNLegend\0\0onRefitClick\0"
    "onReplotClick\0onSavePlotClick\0"
    "onSaveTxtClick\0newFit\0start\0end"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlotWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    2,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,

       0        // eod
};

void PlotWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlotWindow *_t = static_cast<PlotWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onYorNLegend(); break;
        case 1: _t->onRefitClick(); break;
        case 2: _t->onReplotClick(); break;
        case 3: _t->onSavePlotClick(); break;
        case 4: _t->onSaveTxtClick(); break;
        case 5: _t->newFit((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PlotWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_PlotWindow.data,
    qt_meta_data_PlotWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PlotWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlotWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlotWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PlotWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
