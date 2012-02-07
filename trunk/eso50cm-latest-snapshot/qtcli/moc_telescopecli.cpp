/****************************************************************************
** Meta object code from reading C++ file 'telescopecli.h'
**
** Created: Wed May 25 16:08:45 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "telescopecli.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'telescopecli.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TelescopeCli[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      32,   13,   13,   13, 0x08,
      60,   55,   13,   13, 0x08,
      98,   78,   13,   13, 0x08,
     124,   13,   13,   13, 0x08,
     142,   13,   13,   13, 0x08,
     157,   13,   13,   13, 0x08,
     175,   13,   13,   13, 0x08,
     191,   13,   13,   13, 0x08,
     216,  208,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TelescopeCli[] = {
    "TelescopeCli\0\0connectToServer()\0"
    "disconnectFromServer()\0data\0"
    "showData(QString)\0socketError,message\0"
    "displayError(int,QString)\0processOneThing()\0"
    "cliConnected()\0cliDisconnected()\0"
    "theSkyMessage()\0handsetMessage()\0"
    "message\0informationMessage(QString)\0"
};

const QMetaObject TelescopeCli::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_TelescopeCli,
      qt_meta_data_TelescopeCli, 0 }
};

const QMetaObject *TelescopeCli::metaObject() const
{
    return &staticMetaObject;
}

void *TelescopeCli::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TelescopeCli))
        return static_cast<void*>(const_cast< TelescopeCli*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int TelescopeCli::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connectToServer(); break;
        case 1: disconnectFromServer(); break;
        case 2: showData((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: displayError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: processOneThing(); break;
        case 5: cliConnected(); break;
        case 6: cliDisconnected(); break;
        case 7: theSkyMessage(); break;
        case 8: handsetMessage(); break;
        case 9: informationMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
