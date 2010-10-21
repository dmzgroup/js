#ifndef DMZ_V8_QT_UTIL_DOT_H
#define DMZ_V8_QT_UTIL_DOT_H

#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilTypes.h>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <v8.h>


namespace dmz {

   V8Value qvariant_to_v8 (const QVariant &Value);
   V8Value qstring_to_v8 (const QString &Value);
   V8Value qdatetime_to_v8 (const QDateTime &value);

   QVariant v8_to_qvariant (V8Value value);
   QString v8_to_qstring (V8Value value);
   QDateTime v8_to_qdatetime (V8Value value);
};


inline dmz::V8Value
dmz::qstring_to_v8 (const QString &Value) {

   return v8::String::New (qPrintable (Value));;
}


inline QString
dmz::v8_to_qstring (V8Value value) {

   return QString (*(v8::String::AsciiValue (value)));
}

inline QDateTime
dmz::v8_to_qdatetime (V8Value value) {

   QDateTime dt;
   v8::HandleScope scope;
   if (v8_is_valid(value) && value->IsDate ()) {

      v8::Handle<v8::Date> date = v8::Handle<v8::Date>::Cast (value);
      dt = QDateTime::fromTime_t (date->NumberValue () / 1000);
   }
   return dt;
}

inline dmz::V8Value
dmz::qdatetime_to_v8 (const QDateTime &value) {

   v8::HandleScope scope;
   return v8::Date::New ((Float64)value.toTime_t () * 1000.0);
}


#endif // DMZ_V8_QT_OBJECT_DOT_H
