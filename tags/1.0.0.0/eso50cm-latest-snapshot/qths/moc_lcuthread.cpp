/****************************************************************************
** Meta object code from reading C++ file 'lcuthread.h'
**
** Created: Wed Jan 26 00:02:13 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "lcuthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lcuthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_lcuThread[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   11,   10,   10, 0x05,
      53,   33,   10,   10, 0x05,
      72,   10,   10,   10, 0x05,
      87,   10,   10,   10, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_lcuThread[] = {
    "lcuThread\0\0data\0newData(QString)\0"
    "socketError,message\0error(int,QString)\0"
    "runConnected()\0runDisconnected()\0"
};

const QMetaObject lcuThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_lcuThread,
      qt_meta_data_lcuThread, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &lcuThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *lcuThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *lcuThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_lcuThread))
        return static_cast<void*>(const_cast< lcuThread*>(this));
    return QThread::qt_metacast(_clname);
}

int lcuThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void lcuThread::newData(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void lcuThread::error(int _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void lcuThread::runConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void lcuThread::runDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE