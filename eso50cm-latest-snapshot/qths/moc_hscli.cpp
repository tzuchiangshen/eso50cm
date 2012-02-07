/****************************************************************************
** Meta object code from reading C++ file 'hscli.h'
**
** Created: Wed Jan 26 00:02:11 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hscli.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hscli.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_hsCli[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,    7,    6,    6, 0x08,
      53,    6,    6,    6, 0x08,
      81,   73,    6,    6, 0x08,
     103,   73,    6,    6, 0x08,
     125,    6,    6,    6, 0x08,
     144,    6,    6,    6, 0x08,
     164,    6,    6,    6, 0x08,
     183,    6,    6,    6, 0x08,
     203,    6,    6,    6, 0x08,
     218,    6,    6,    6, 0x08,
     234,    6,    6,    6, 0x08,
     249,    6,    6,    6, 0x08,
     265,    6,    6,    6, 0x08,
     279,    6,    6,    6, 0x08,
     294,    6,    6,    6, 0x08,
     308,    6,    6,    6, 0x08,
     323,    6,    6,    6, 0x08,
     341,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_hsCli[] = {
    "hsCli\0\0socketError,message\0"
    "displayError(int,QString)\0handsetMessage(int)\0"
    "message\0alphaMessage(QString)\0"
    "deltaMessage(QString)\0alphaPressed(char)\0"
    "alphaReleased(char)\0deltaPressed(char)\0"
    "deltaReleased(char)\0northPressed()\0"
    "northReleased()\0southPressed()\0"
    "southReleased()\0eastPressed()\0"
    "eastReleased()\0westPressed()\0"
    "westReleased()\0connectHSDevice()\0"
    "disconnectHSDevice()\0"
};

const QMetaObject hsCli::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_hsCli,
      qt_meta_data_hsCli, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &hsCli::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *hsCli::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *hsCli::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_hsCli))
        return static_cast<void*>(const_cast< hsCli*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int hsCli::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: displayError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: handsetMessage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: alphaMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: deltaMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: alphaPressed((*reinterpret_cast< char(*)>(_a[1]))); break;
        case 5: alphaReleased((*reinterpret_cast< char(*)>(_a[1]))); break;
        case 6: deltaPressed((*reinterpret_cast< char(*)>(_a[1]))); break;
        case 7: deltaReleased((*reinterpret_cast< char(*)>(_a[1]))); break;
        case 8: northPressed(); break;
        case 9: northReleased(); break;
        case 10: southPressed(); break;
        case 11: southReleased(); break;
        case 12: eastPressed(); break;
        case 13: eastReleased(); break;
        case 14: westPressed(); break;
        case 15: westReleased(); break;
        case 16: connectHSDevice(); break;
        case 17: disconnectHSDevice(); break;
        default: ;
        }
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
