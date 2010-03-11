#ifndef DMZ_JS_V8_UTIL_CONVERT_DOT_H
#define DMZ_JS_V8_UTIL_CONVERT_DOT_H

#include <dmzJsV8UtilExport.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzTypesBase.h>
#include <dmzTypesString.h>

#include <v8.h>

namespace dmz {

class HandleContainer;

template <class T> T *
v8_to_pointer (V8Value value) {

   T *result (0);

   if ((value.IsEmpty () == false) && value->IsExternal ()) {

      V8External external = V8External::Cast (value);
      if (external.IsEmpty () == false) { result = (T *)external->Value (); }
   }

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

Boolean
v8_to_boolean (V8Value value);

String
v8_to_string (V8Value value);

Float64
v8_to_number (V8Value value);

Int32
v8_to_int32 (V8Value value);

Int64
v8_to_int64 (V8Value value);

UInt32
v8_to_uint32 (V8Value value);

UInt32
v8_to_handle (V8Value value);

V8Object
v8_to_object (V8Value value);

V8Function
v8_to_function (V8Value value);

V8Array
v8_to_array (V8Value value);

DMZ_JS_V8_UTIL_LINK_SYMBOL V8Array
v8_to_array (HandleContainer &container);

Boolean
v8_is_object (V8Value value);

Boolean
v8_is_function (V8Value value);

Boolean
v8_is_array (V8Value value);

};


inline dmz::Boolean
dmz::v8_to_boolean (V8Value value) {

   Boolean result (False);

   if (value.IsEmpty () == false) { result = value->BooleanValue (); }

   return result;
}


inline dmz::String
dmz::v8_to_string (V8Value value) {

   return String (*(v8::String::AsciiValue (value)));
}


inline dmz::Float64
dmz::v8_to_number (V8Value value) {

   Float64 result (0.0);

   if ((value.IsEmpty () == false) && value->IsNumber ()) {

      result = value->NumberValue ();
   }

   return result;
}


inline dmz::Int32
dmz::v8_to_int32 (V8Value value) {

   Int32 result (0);

   if ((value.IsEmpty () == false) && value->IsNumber ()) {

      result = (Int32)value->IntegerValue ();
   }

   return result;
}


inline dmz::Int64
dmz::v8_to_int64 (V8Value value) {

   Int64 result (0);

   if ((value.IsEmpty () == false) && value->IsNumber ()) {

      result = value->IntegerValue ();
   }

   return result;
}


inline dmz::UInt32
dmz::v8_to_uint32 (V8Value value) {

   UInt32 result (0);

   if ((value.IsEmpty () == false) && value->IsNumber ()) {

      result = value->Uint32Value ();
   }

   return result;
}


inline dmz::Handle
dmz::v8_to_handle (V8Value value) {

   Handle result (0);

   if ((value.IsEmpty () == false) && value->IsNumber ()) {

      result = value->Uint32Value ();
   }

   return result;
}


inline dmz::V8Object
dmz::v8_to_object (V8Value value) {

   V8Object result;

   if ((value.IsEmpty () == false) && value->IsObject ()) {

      result = V8Object::Cast (value);
   }

   return result;
}


inline dmz::V8Function
dmz::v8_to_function (V8Value value) {

   V8Function result;

   if ((value.IsEmpty () == false) && value->IsFunction ()) { 

      result = V8Function::Cast (value);
   }

   return result;
}


inline dmz::V8Array
dmz::v8_to_array (V8Value value) {

   V8Array result;

   if ((value.IsEmpty () == false) && value->IsArray ()) {

      result = V8Array::Cast (value);
   }

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
