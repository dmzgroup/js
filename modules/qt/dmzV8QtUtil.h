#ifndef DMZ_V8_QT_UTIL_DOT_H
#define DMZ_V8_QT_UTIL_DOT_H

#include <dmzJsV8UtilTypes.h>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <v8.h>


namespace dmz {

   V8Value qvariant_to_v8 (const QVariant &Value);
   V8Value qstring_to_v8 (const QString &Value);

   QVariant v8_to_qvariant (V8Value value);
   QString v8_to_qstring (V8Value value);
};


inline dmz::V8Value
dmz::qstring_to_v8 (const QString &Value) {

   return v8::String::New (qPrintable (Value));;
}


inline QString
dmz::v8_to_qstring (V8Value value) {

   return QString (*(v8::String::AsciiValue (value)));
}


#endif // DMZ_V8_QT_OBJECT_DOT_H
