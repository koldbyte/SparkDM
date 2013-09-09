/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[23];
    char stringdata[605];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 15),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 18),
QT_MOC_LITERAL(4, 47, 26),
QT_MOC_LITERAL(5, 74, 23),
QT_MOC_LITERAL(6, 98, 26),
QT_MOC_LITERAL(7, 125, 38),
QT_MOC_LITERAL(8, 164, 17),
QT_MOC_LITERAL(9, 182, 33),
QT_MOC_LITERAL(10, 216, 17),
QT_MOC_LITERAL(11, 234, 17),
QT_MOC_LITERAL(12, 252, 29),
QT_MOC_LITERAL(13, 282, 31),
QT_MOC_LITERAL(14, 314, 25),
QT_MOC_LITERAL(15, 340, 34),
QT_MOC_LITERAL(16, 375, 44),
QT_MOC_LITERAL(17, 420, 3),
QT_MOC_LITERAL(18, 424, 48),
QT_MOC_LITERAL(19, 473, 31),
QT_MOC_LITERAL(20, 505, 33),
QT_MOC_LITERAL(21, 539, 39),
QT_MOC_LITERAL(22, 579, 24)
    },
    "MainWindow\0UpdateInterface\0\0"
    "ADownloadCompleted\0on_actionAdd_New_triggered\0"
    "on_actionQuit_triggered\0"
    "on_actionOptions_triggered\0"
    "on_categoriesTree_itemSelectionChanged\0"
    "TrayIconActivated\0QSystemTrayIcon::ActivationReason\0"
    "ShowHideInterface\0ReadUrlAndProcess\0"
    "on_actionProperties_triggered\0"
    "on_actionDownload_Now_triggered\0"
    "on_actionRemove_triggered\0"
    "on_actionHide_Categories_triggered\0"
    "on_categoriesTree_customContextMenuRequested\0"
    "pos\0on_dockWidgetContents_customContextMenuRequested\0"
    "on_actionTo_text_file_triggered\0"
    "on_actionFrom_text_file_triggered\0"
    "on_actionDelete_All_Completed_triggered\0"
    "on_actionAbout_triggered\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x0a,
       3,    1,  110,    2, 0x0a,
       4,    0,  113,    2, 0x08,
       5,    0,  114,    2, 0x08,
       6,    0,  115,    2, 0x08,
       7,    0,  116,    2, 0x08,
       8,    1,  117,    2, 0x08,
      10,    0,  120,    2, 0x08,
      11,    0,  121,    2, 0x08,
      12,    0,  122,    2, 0x08,
      13,    0,  123,    2, 0x08,
      14,    0,  124,    2, 0x08,
      15,    0,  125,    2, 0x08,
      16,    1,  126,    2, 0x08,
      18,    1,  129,    2, 0x08,
      19,    0,  132,    2, 0x08,
      20,    0,  133,    2, 0x08,
      21,    0,  134,    2, 0x08,
      22,    0,  135,    2, 0x08,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Int, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int, 0x80000000 | 9,    2,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   17,
    QMetaType::Void, QMetaType::QPoint,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: { int _r = _t->UpdateInterface();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->ADownloadCompleted((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: _t->on_actionAdd_New_triggered(); break;
        case 3: _t->on_actionQuit_triggered(); break;
        case 4: _t->on_actionOptions_triggered(); break;
        case 5: _t->on_categoriesTree_itemSelectionChanged(); break;
        case 6: { int _r = _t->TrayIconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: { int _r = _t->ShowHideInterface();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->ReadUrlAndProcess();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: _t->on_actionProperties_triggered(); break;
        case 10: _t->on_actionDownload_Now_triggered(); break;
        case 11: _t->on_actionRemove_triggered(); break;
        case 12: _t->on_actionHide_Categories_triggered(); break;
        case 13: _t->on_categoriesTree_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 14: _t->on_dockWidgetContents_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 15: _t->on_actionTo_text_file_triggered(); break;
        case 16: _t->on_actionFrom_text_file_triggered(); break;
        case 17: _t->on_actionDelete_All_Completed_triggered(); break;
        case 18: _t->on_actionAbout_triggered(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
