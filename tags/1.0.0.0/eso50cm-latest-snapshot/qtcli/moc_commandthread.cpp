/****************************************************************************
** Meta object code from reading C++ file 'commandthread.h'
**
** Created: Wed May 25 16:08:58 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "commandthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'commandthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_commandThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x05,
      57,   37,   14,   14, 0x05,
      76,   14,   14,   14, 0x05,
      91,   14,   14,   14, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_commandThread[] = {
    "commandThread\0\0data\0newData(QString)\0"
    "socketError,message\0error(int,QString)\0"
    "runConnected()\0runDisconnected()\0"
};

const QMetaObject commandThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_commandThread,
      qt_meta_data_commandThread, 0 }
};

const QMetaObject *commandThread::metaObject() const
{
    return &staticMetaObject;
}

void *commandThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_commandThread))
        return static_cast<void*>(const_cast< commandThread*>(this));
    return QThread::qt_metacast(_clname);
}

int commandThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newData((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: error((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: runConnected(); break;
        case 3: runDisconnected(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void commandThread::newData(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void commandThread::error(int _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void commandThread::runConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void commandThread::runDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
