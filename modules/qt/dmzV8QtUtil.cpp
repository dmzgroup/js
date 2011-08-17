#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"


dmz::V8Value
dmz::qpoint_to_v8 (QPoint pos) {

   v8::HandleScope scope;
   dmz::V8Object result;

   result = v8::Object::New ();
   result->Set(v8::String::NewSymbol ("x"), v8::Number::New (pos.x ()));
   result->Set(v8::String::NewSymbol ("y"), v8::Number::New (pos.y ()));

   return scope.Close (result);
}


dmz::V8Value
dmz::qpointf_to_v8 (QPointF pos) {

   v8::HandleScope scope;
   dmz::V8Object result;

   result = v8::Object::New ();
   result->Set(v8::String::NewSymbol ("x"), v8::Number::New (pos.x ()));
   result->Set(v8::String::NewSymbol ("y"), v8::Number::New (pos.y ()));

   return scope.Close (result);
}

dmz::V8Value
dmz::qsize_to_v8 (const QSize &Value) {

   v8::HandleScope scope;
   dmz::V8Object result;

   result = v8::Object::New ();
   result->Set (v8::String::NewSymbol ("width"), v8::Number::New (Value.width ()));
   result->Set (v8::String::NewSymbol ("height"), v8::Number::New (Value.height ()));

   return scope.Close (result);
}


QSize
dmz::v8_to_qsize (dmz::V8Value value) {

   QSize result;

   if (!value.IsEmpty ()) {

      dmz::V8Object obj = dmz::v8_to_object (value);
      if (!obj.IsEmpty ()) {

         qreal w, h;
         w = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("width")));
         h = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("height")));
         result = QSize(w, h);
      }
   }
   return result;
}

QFont
dmz::v8_to_qfont (const dmz::V8Value value) {

   QFont result;

   if (!value.IsEmpty ()) {

      dmz::V8Object obj = dmz::v8_to_object (value);
      if (!obj.IsEmpty ()) {
         QString family;
         qreal size, weight;
         bool italic;
         family = dmz::v8_to_qstring (obj->Get (v8::String::NewSymbol ("font")));
         size = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("size")));
         weight = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("weight")));
         italic = dmz::v8_to_boolean (obj->Get (v8::String::NewSymbol ("italic")));
         result = QFont (family, size, weight, italic);
      }
   }
   return result;
}

dmz::V8Value
dmz::qfont_to_v8 (const QFont &Value) {

   v8::HandleScope scope;
   dmz::V8Object result;
   result = v8::Object::New ();
   result->Set (
      v8::String::NewSymbol ("font"),
      v8::String::New (qPrintable (Value.family ())));
   result->Set (v8::String::NewSymbol ("size"), v8::Number::New (Value.pointSize ()));
   result->Set (v8::String::NewSymbol ("weight"), v8::Number::New (Value.weight ()));
   result->Set (v8::String::NewSymbol ("italic"), v8::Boolean::New (Value.italic ()));
   return scope.Close (result);
}


dmz::V8Value
dmz::qrectf_to_v8 (const QRectF &Value) {

   v8::HandleScope scope;
   dmz::V8Object result;

   result = v8::Object::New ();
   result->Set (v8::String::NewSymbol ("x"), v8::Number::New (Value.x ()));
   result->Set (v8::String::NewSymbol ("y"), v8::Number::New (Value.y ()));
   result->Set (v8::String::NewSymbol ("width"), v8::Number::New (Value.width ()));
   result->Set (v8::String::NewSymbol ("height"), v8::Number::New (Value.height ()));

   return scope.Close (result);
}


QRectF
dmz::v8_to_qrectf (dmz::V8Value value) {

   QRectF result;

   if (!value.IsEmpty ()) {

      dmz::V8Object obj = dmz::v8_to_object (value);
      if (!obj.IsEmpty ()) {

         qreal x, y, w, h;
         x = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("x")));
         y = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("y")));
         w = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("width")));
         h = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("height")));
         result = QRectF(x, y, w, h);
      }
   }
   return result;
}


dmz::V8Value
dmz::qrect_to_v8 (const QRect &Value) {

   v8::HandleScope scope;
   dmz::V8Object result;

   result = v8::Object::New ();
   result->Set (v8::String::NewSymbol ("x"), v8::Number::New (Value.x ()));
   result->Set (v8::String::NewSymbol ("y"), v8::Number::New (Value.y ()));
   result->Set (v8::String::NewSymbol ("width"), v8::Number::New (Value.width ()));
   result->Set (v8::String::NewSymbol ("height"), v8::Number::New (Value.height ()));

   return scope.Close (result);
}


QRect
dmz::v8_to_qrect (dmz::V8Value value) {

   QRect result;

   if (!value.IsEmpty ()) {

      dmz::V8Object obj = dmz::v8_to_object (value);
      if (!obj.IsEmpty ()) {

         qreal x, y, w, h;
         x = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("x")));
         y = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("y")));
         w = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("width")));
         h = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("height")));
         result = QRect(x, y, w, h);
      }
   }
   return result;
}


dmz::V8Value
dmz::qcolor_to_v8 (const QColor &Value) {

   v8::HandleScope scope;
   dmz::V8Object result;

   result = v8::Object::New ();
   result->Set (v8::String::NewSymbol ("r"), v8::Number::New (Value.redF ()));
   result->Set (v8::String::NewSymbol ("g"), v8::Number::New (Value.greenF ()));
   result->Set (v8::String::NewSymbol ("b"), v8::Number::New (Value.blueF ()));

   return scope.Close (result);
}


QColor
dmz::v8_to_qcolor (dmz::V8Value value) {

   QColor result;

   if (!value.IsEmpty ()) {

      dmz::V8Object obj = dmz::v8_to_object (value);
      if (!obj.IsEmpty ()) {

         qreal r, g, b;
         r = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("r")));
         g = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("g")));
         b = dmz::v8_to_number (obj->Get (v8::String::NewSymbol ("b")));
         result.setRgbF (r, g, b);
      }
   }
   return result;
}


dmz::V8Value
dmz::qvariant_to_v8 (const QVariant &Value) {

   V8Value newValue = v8::Undefined ();

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


QStringList
dmz::v8_to_qstringlist (V8Value value) {

   QStringList list;

   if (!value.IsEmpty () && !value->IsUndefined ()) {

      V8Array array = v8_to_array (value);
      const uint32_t Length = array->Length ();
      for (uint32_t ix = 0; ix < Length; ix++) {

         QString value = v8_to_qstring (array->Get (v8::Integer::NewFromUnsigned (ix)));
         list.append (value);
      }
   }

   return list;
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
   else if (StandardKey == QLatin1String ("saveas")) {

      shortcut = QKeySequence::SaveAs;
   }
   else if (StandardKey == QLatin1String ("print")) { shortcut = QKeySequence::Print; }
   else if (StandardKey == QLatin1String ("zoomin")) { shortcut = QKeySequence::ZoomIn; }
   else if (StandardKey == QLatin1String ("zoomout")) {

      shortcut = QKeySequence::ZoomOut;
   }

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


dmz::V8Variant::V8Variant (const V8Variant &Type) :
      valueRef ((V8ValueRef *)Type.valueRef) {

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
