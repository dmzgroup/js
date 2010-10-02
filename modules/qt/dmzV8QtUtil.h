#ifndef DMZ_V8_QT_UTIL_DOT_H
#define DMZ_V8_QT_UTIL_DOT_H

#include <dmzJsV8UtilTypes.h>
#include <QtCore/QVariant>
#include <v8.h>

namespace dmz {

   V8Value to_v8_value (const QVariant &Value);
   QVariant to_qt_variant (V8Value value);
};


#endif // DMZ_V8_QT_OBJECT_DOT_H
