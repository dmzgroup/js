#ifndef DMZ_V8_QT_UTIL_DOT_H
#define DMZ_V8_QT_UTIL_DOT_H

#include <dmzJsV8UtilTypes.h>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <v8.h>


namespace dmz {

   V8Value to_v8_value (const QVariant &Value);
   QVariant to_qt_variant (V8Value value);

   V8Value to_v8_value (const QString &Value);
   QString v8_to_qstring (V8Value value);
};


inline dmz::V8Value
dmz::to_v8_value (const QString &Value) {

   return v8::String::New (qPrintable (Value));;
}


inline QString
dmz::v8_to_qstring (V8Value value) {

   return QString (*(v8::String::AsciiValue (value)));
}


#endif // DMZ_V8_QT_OBJECT_DOT_H
