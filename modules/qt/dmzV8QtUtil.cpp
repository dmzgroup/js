#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"


dmz::V8Value
dmz::qvariant_to_v8 (const QVariant &Value) {

   V8Value newValue;

   if (Value.isValid ()) {

      if (Value.type () == QVariant::Bool) {

         newValue = v8::Boolean::New (Value.toBool ());
      }
      else if (Value.type () == QVariant::Int) {

         newValue = v8::Integer::New (Value.toInt ());
      }
      else if (Value.type () == QVariant::UInt) {

         newValue = v8::Integer::NewFromUnsigned (Value.toUInt ());
      }
      else if (Value.type () == QVariant::Double) {

         newValue = v8::Number::New (Value.toDouble ());
      }
      else if (Value.type () == QVariant::String) {

         newValue = qstring_to_v8 (Value.toString ());
      }
   }

   return newValue;
}


QVariant
dmz::v8_to_qvariant (V8Value value) {

   QVariant result;

   if (!value.IsEmpty () && !value->IsUndefined ()) {

      if (value->IsBoolean ()) { result = v8_to_boolean (value); }
      else if (value->IsInt32 ()) { result = v8_to_int32 (value); }
      else if (value->IsUint32 ()) { result = v8_to_uint32 (value); }
      else if (value->IsNumber ()) { result = v8_to_number (value); }
      else if (value->IsString ()) { result = v8_to_qstring (value); }
   }

   return result;
}

