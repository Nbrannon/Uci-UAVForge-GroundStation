/****************************************************************************
** Meta object code from reading C++ file 'mapexecution.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UAV-Forge-New/mapexecution.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapexecution.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MapExecution_t {
    QByteArrayData data[18];
    char stringdata[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MapExecution_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MapExecution_t qt_meta_stringdata_MapExecution = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MapExecution"
QT_MOC_LITERAL(1, 13, 9), // "addNewMap"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 6), // "setMap"
QT_MOC_LITERAL(4, 31, 14), // "QList<QString>"
QT_MOC_LITERAL(5, 46, 4), // "list"
QT_MOC_LITERAL(6, 51, 12), // "plotPosition"
QT_MOC_LITERAL(7, 64, 3), // "lat"
QT_MOC_LITERAL(8, 68, 3), // "lng"
QT_MOC_LITERAL(9, 72, 13), // "newTelemCoord"
QT_MOC_LITERAL(10, 86, 11), // "coordString"
QT_MOC_LITERAL(11, 98, 14), // "sendFlightPlan"
QT_MOC_LITERAL(12, 113, 16), // "addClickListener"
QT_MOC_LITERAL(13, 130, 13), // "finishClicked"
QT_MOC_LITERAL(14, 144, 17), // "returnHomeClicked"
QT_MOC_LITERAL(15, 162, 13), // "cancelClicked"
QT_MOC_LITERAL(16, 176, 11), // "stopClicked"
QT_MOC_LITERAL(17, 188, 21) // "on_pushButton_clicked"

    },
    "MapExecution\0addNewMap\0\0setMap\0"
    "QList<QString>\0list\0plotPosition\0lat\0"
    "lng\0newTelemCoord\0coordString\0"
    "sendFlightPlan\0addClickListener\0"
    "finishClicked\0returnHomeClicked\0"
    "cancelClicked\0stopClicked\0"
    "on_pushButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MapExecution[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a /* Public */,
       3,    1,   70,    2, 0x0a /* Public */,
       6,    2,   73,    2, 0x0a /* Public */,
       9,    1,   78,    2, 0x0a /* Public */,
      11,    0,   81,    2, 0x0a /* Public */,
      12,    0,   82,    2, 0x08 /* Private */,
      13,    0,   83,    2, 0x08 /* Private */,
      14,    0,   84,    2, 0x08 /* Private */,
      15,    0,   85,    2, 0x08 /* Private */,
      16,    0,   86,    2, 0x08 /* Private */,
      17,    0,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    7,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MapExecution::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MapExecution *_t = static_cast<MapExecution *>(_o);
        switch (_id) {
        case 0: _t->addNewMap(); break;
        case 1: _t->setMap((*reinterpret_cast< QList<QString>(*)>(_a[1]))); break;
        case 2: _t->plotPosition((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->newTelemCoord((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->sendFlightPlan(); break;
        case 5: _t->addClickListener(); break;
        case 6: _t->finishClicked(); break;
        case 7: _t->returnHomeClicked(); break;
        case 8: _t->cancelClicked(); break;
        case 9: _t->stopClicked(); break;
        case 10: _t->on_pushButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QString> >(); break;
            }
            break;
        }
    }
}

const QMetaObject MapExecution::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MapExecution.data,
      qt_meta_data_MapExecution,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MapExecution::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapExecution::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MapExecution.stringdata))
        return static_cast<void*>(const_cast< MapExecution*>(this));
    return QWidget::qt_metacast(_clname);
}

int MapExecution::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
