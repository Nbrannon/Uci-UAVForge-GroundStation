/****************************************************************************
** Meta object code from reading C++ file 'mapplanning.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UAV-Forge-New/mapplanning.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapplanning.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MapPlanning_t {
    QByteArrayData data[13];
    char stringdata[179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MapPlanning_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MapPlanning_t qt_meta_stringdata_MapPlanning = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MapPlanning"
QT_MOC_LITERAL(1, 12, 15), // "addPointToTable"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3), // "lat"
QT_MOC_LITERAL(4, 33, 3), // "lng"
QT_MOC_LITERAL(5, 37, 15), // "on_back_clicked"
QT_MOC_LITERAL(6, 53, 17), // "on_addRow_clicked"
QT_MOC_LITERAL(7, 71, 20), // "on_removeRow_clicked"
QT_MOC_LITERAL(8, 92, 18), // "on_execute_clicked"
QT_MOC_LITERAL(9, 111, 17), // "on_update_clicked"
QT_MOC_LITERAL(10, 129, 20), // "on_clearPath_clicked"
QT_MOC_LITERAL(11, 150, 16), // "addClickListener"
QT_MOC_LITERAL(12, 167, 11) // "closeWindow"

    },
    "MapPlanning\0addPointToTable\0\0lat\0lng\0"
    "on_back_clicked\0on_addRow_clicked\0"
    "on_removeRow_clicked\0on_execute_clicked\0"
    "on_update_clicked\0on_clearPath_clicked\0"
    "addClickListener\0closeWindow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MapPlanning[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x0a /* Public */,
       5,    0,   64,    2, 0x08 /* Private */,
       6,    0,   65,    2, 0x08 /* Private */,
       7,    0,   66,    2, 0x08 /* Private */,
       8,    0,   67,    2, 0x08 /* Private */,
       9,    0,   68,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,
      11,    0,   70,    2, 0x08 /* Private */,
      12,    0,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MapPlanning::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MapPlanning *_t = static_cast<MapPlanning *>(_o);
        switch (_id) {
        case 0: _t->addPointToTable((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->on_back_clicked(); break;
        case 2: _t->on_addRow_clicked(); break;
        case 3: _t->on_removeRow_clicked(); break;
        case 4: _t->on_execute_clicked(); break;
        case 5: _t->on_update_clicked(); break;
        case 6: _t->on_clearPath_clicked(); break;
        case 7: _t->addClickListener(); break;
        case 8: _t->closeWindow(); break;
        default: ;
        }
    }
}

const QMetaObject MapPlanning::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MapPlanning.data,
      qt_meta_data_MapPlanning,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MapPlanning::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapPlanning::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MapPlanning.stringdata))
        return static_cast<void*>(const_cast< MapPlanning*>(this));
    return QDialog::qt_metacast(_clname);
}

int MapPlanning::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
