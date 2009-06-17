#ifndef DMZ_JS_V8_UTIL_CONVERT_DOT_H
#define DMZ_JS_V8_UTIL_CONVERT_DOT_H

#include <dmzJsV8UtilExport.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzTypesMatrix.h>
#include <dmzTypesVector.h>

#include <v8.h>

namespace dmz {

DMZ_JS_V8_UTIL_LINK_SYMBOL V8Value
to_v8_vector (const Vector &Value, V8Object root = V8Object ());

DMZ_JS_V8_UTIL_LINK_SYMBOL Vector
to_dmz_vector (V8Value value);

DMZ_JS_V8_UTIL_LINK_SYMBOL V8Value
to_v8_matrix (const Matrix &Value, V8Object root = V8Object ());

DMZ_JS_V8_UTIL_LINK_SYMBOL Matrix
to_dmz_matrix (V8Value value);

Float64
to_number (V8Value value);

Boolean
is_object (V8Value value);

Boolean
is_function (V8Value value);

Boolean
is_array (V8Value value);

};

inline dmz::Float64
dmz::to_number (V8Value value) {

   Float64 result (0.0);

   if ((value.IsEmpty () == false) && value->IsNumber ()) {

      result = value->NumberValue ();
   }

   return result;
}


inline dmz::Boolean
dmz::is_object (V8Value value) {

   return (value.IsEmpty () == false) && value->IsObject ();
}


inline dmz::Boolean
dmz::is_function (V8Value value) {

   return (value.IsEmpty () == false) && value->IsFunction ();
}


inline dmz::Boolean
dmz::is_array (V8Value value) {

   return (value.IsEmpty () == false) && value->IsArray ();
}

#endif // DMZ_JS_V8_UTIL_CONVERT_DOT_H
