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


dmz::V8Value
dmz::qstringlist_to_v8 (const QStringList &Value) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   if (Value.count ()) {

      V8Array array = v8::Array::New (Value.count ());

      Int32 count = 0;
      foreach (QString item, Value) {

         array->Set (v8::Integer::New (count++), qstring_to_v8 (item));
      }

      result = array;
   }

   return scope.Close (result);
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


QKeySequence
dmz::v8_to_qkeysequence (V8Value value) {

   QKeySequence shortcut (v8_to_qstring (value));

   const QString StandardKey (v8_to_qstring (value).toLower ());

   if (StandardKey == "undo") { shortcut = QKeySequence::Undo; }
   else if (StandardKey == QLatin1String ("redo")) { shortcut = QKeySequence::Redo; }
   else if (StandardKey == QLatin1String ("new")) { shortcut = QKeySequence::New; }
   else if (StandardKey == QLatin1String ("open")) { shortcut = QKeySequence::Open; }
   else if (StandardKey == QLatin1String ("save")) { shortcut = QKeySequence::Save; }
   else if (StandardKey == QLatin1String ("saveas")) { shortcut = QKeySequence::SaveAs; }
   else if (StandardKey == QLatin1String ("print")) { shortcut = QKeySequence::Print; }
   else if (StandardKey == QLatin1String ("zoomin")) { shortcut = QKeySequence::ZoomIn; }
   else if (StandardKey == QLatin1String ("zoomout")) { shortcut = QKeySequence::ZoomOut; }

   return shortcut;
}


// V8ValueRef

dmz::V8ValueRef::V8ValueRef (V8Value theValue,  QList<V8ValueRef *> &theList) :
      deleteList (theList) {

   value = V8ValuePersist::New (theValue);
}


dmz::V8ValueRef::~V8ValueRef () {

   value.Dispose (); value.Clear ();
}


void
dmz::V8ValueRef::_ref_count_is_zero () {

   deleteList.append (this);
}


// V8Variant

dmz::V8Variant::V8Variant () : valueRef (0) {;}


dmz::V8Variant::V8Variant (V8ValueRef *obj) : valueRef (obj) {

   if (valueRef) { valueRef->ref (); }
}


dmz::V8Variant::V8Variant (const V8Variant &Type) : valueRef ((V8ValueRef *)Type.valueRef) {

   if (valueRef) { valueRef->ref (); }
}


dmz::V8Variant::~V8Variant () {

   if (valueRef) { valueRef->unref (); valueRef = 0; }
}


dmz::V8Variant &
dmz::V8Variant::operator= (const V8Variant &Type) {

   if (valueRef) { valueRef->unref (); valueRef = 0; }
   valueRef = (V8ValueRef *)Type.valueRef;
   if (valueRef) { valueRef->ref (); }

   return *this;
}
