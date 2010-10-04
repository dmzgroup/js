#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"

#include <QtCore/QDebug>


dmz::V8Value
dmz::to_v8_value (const QVariant &Value) {

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

         newValue = v8::String::New (qPrintable (Value.toString ()));
      }
   }

   return newValue;
}


QVariant
dmz::to_qt_variant (V8Value value) {

   QVariant result;

   if (!value.IsEmpty () && !value->IsUndefined ()) {

      if (value->IsBoolean ()) { result = v8_to_boolean (value); }
      else if (value->IsInt32 ()) { result = v8_to_int32 (value); }
      else if (value->IsUint32 ()) { result = v8_to_uint32 (value); }
      else if (value->IsNumber ()) { result = v8_to_number (value); }
      else if (value->IsString ()) { result = v8_to_string (value).get_buffer (); }
   }

   return result;
}
