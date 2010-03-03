#ifndef DMZ_JS_V8_UTIL_CONVERT_DOT_H
#define DMZ_JS_V8_UTIL_CONVERT_DOT_H

#include <dmzJsV8UtilExport.h>
#include <dmzJsV8UtilTypes.h>

#include <v8.h>

namespace dmz {

template <class T> T *
v8_to_pointer (V8Value value) {

   T *result (0);
   V8External external = V8External::Cast (value);
   if (external.IsEmpty () == false) { result = (T *)external->Value (); }
   return result;
}

template <class T> T *
v8_to_pointer (V8Object obj, const int Offset) {

   T *result (0);

   if (obj.IsEmpty () == false) {

      result = v8_to_pointer<T> (obj->GetInternalField (Offset));
   }

   return result;
}

template <class T> T *
v8_to_pointer (V8Value value, const int Offset) {

   T *result (0);
   V8Object obj = V8Object::Cast (value);
   if (obj.IsEmpty () == false) { result = v8_to_pointer<T> (obj, Offset); }
   return result;
}

String
v8_to_string (V8Value value);

Float64
v8_to_number (V8Value value);

UInt32
v8_to_uint32 (V8Value value);

Boolean
v8_is_object (V8Value value);

Boolean
v8_is_function (V8Value value);

Boolean
v8_is_array (V8Value value);

};


inline dmz::String
dmz::v8_to_string (V8Value value) {

   return String (*(v8::String::AsciiValue (value)));
}


inline dmz::Float64
dmz::v8_to_number (V8Value value) {

   Float64 result (0.0);

   if (value.IsEmpty () == false) { result = value->NumberValue (); }

   return result;
}


inline dmz::UInt32
dmz::v8_to_uint32 (V8Value value) {

   UInt32 result (0.0);

   if (value.IsEmpty () == false) { result = value->Uint32Value (); }

   return result;
}


inline dmz::Boolean
dmz::v8_is_object (V8Value value) {

   return (value.IsEmpty () == false) && value->IsObject ();
}


inline dmz::Boolean
dmz::v8_is_function (V8Value value) {

   return (value.IsEmpty () == false) && value->IsFunction ();
}


inline dmz::Boolean
dmz::v8_is_array (V8Value value) {

   return (value.IsEmpty () == false) && value->IsArray ();
}

#endif // DMZ_JS_V8_UTIL_CONVERT_DOT_H
