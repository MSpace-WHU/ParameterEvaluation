/****************************************************************************
** Meta object code from reading C++ file 'MatchingFeatureEvaluation.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/MatchingFeatureEvaluation.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MatchingFeatureEvaluation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MatchingFeatureEvaluation_t {
    uint offsetsAndSizes[26];
    char stringdata0[26];
    char stringdata1[6];
    char stringdata2[1];
    char stringdata3[33];
    char stringdata4[35];
    char stringdata5[25];
    char stringdata6[19];
    char stringdata7[15];
    char stringdata8[34];
    char stringdata9[12];
    char stringdata10[11];
    char stringdata11[18];
    char stringdata12[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MatchingFeatureEvaluation_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_MatchingFeatureEvaluation_t qt_meta_stringdata_MatchingFeatureEvaluation = {
    {
        QT_MOC_LITERAL(0, 25),  // "MatchingFeatureEvaluation"
        QT_MOC_LITERAL(26, 5),  // "apply"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 32),  // "selectedInputFileOfPredictedTree"
        QT_MOC_LITERAL(66, 34),  // "selectedInputFileOfReferencee..."
        QT_MOC_LITERAL(101, 24),  // "selectedInputFileOfIndex"
        QT_MOC_LITERAL(126, 18),  // "selectedOutputFile"
        QT_MOC_LITERAL(145, 14),  // "processingData"
        QT_MOC_LITERAL(160, 33),  // "std::shared_ptr<MatchingFeatu..."
        QT_MOC_LITERAL(194, 11),  // "_matching3D"
        QT_MOC_LITERAL(206, 10),  // "QTextEdit*"
        QT_MOC_LITERAL(217, 17),  // "outputInformation"
        QT_MOC_LITERAL(235, 14)   // "output_English"
    },
    "MatchingFeatureEvaluation",
    "apply",
    "",
    "selectedInputFileOfPredictedTree",
    "selectedInputFileOfReferenceedTree",
    "selectedInputFileOfIndex",
    "selectedOutputFile",
    "processingData",
    "std::shared_ptr<MatchingFeature>&",
    "_matching3D",
    "QTextEdit*",
    "outputInformation",
    "output_English"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MatchingFeatureEvaluation[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x08,    1 /* Private */,
       3,    0,   51,    2, 0x08,    2 /* Private */,
       4,    0,   52,    2, 0x08,    3 /* Private */,
       5,    0,   53,    2, 0x08,    4 /* Private */,
       6,    0,   54,    2, 0x08,    5 /* Private */,
       7,    3,   55,    2, 0x08,    6 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 10, QMetaType::Bool,    9,   11,   12,

       0        // eod
};

void MatchingFeatureEvaluation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MatchingFeatureEvaluation *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->apply(); break;
        case 1: _t->selectedInputFileOfPredictedTree(); break;
        case 2: _t->selectedInputFileOfReferenceedTree(); break;
        case 3: _t->selectedInputFileOfIndex(); break;
        case 4: _t->selectedOutputFile(); break;
        case 5: _t->processingData((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<MatchingFeature>&>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QTextEdit*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTextEdit* >(); break;
            }
            break;
        }
    }
}

const QMetaObject MatchingFeatureEvaluation::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_MatchingFeatureEvaluation.offsetsAndSizes,
    qt_meta_data_MatchingFeatureEvaluation,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MatchingFeatureEvaluation_t
, QtPrivate::TypeAndForceComplete<MatchingFeatureEvaluation, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::shared_ptr<MatchingFeature> &, std::false_type>, QtPrivate::TypeAndForceComplete<QTextEdit *, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>


>,
    nullptr
} };


const QMetaObject *MatchingFeatureEvaluation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MatchingFeatureEvaluation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MatchingFeatureEvaluation.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::MatchingFeatureEvaluation"))
        return static_cast< Ui::MatchingFeatureEvaluation*>(this);
    return QDialog::qt_metacast(_clname);
}

int MatchingFeatureEvaluation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
