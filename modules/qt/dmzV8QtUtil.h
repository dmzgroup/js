#ifndef DMZ_V8_QT_UTIL_DOT_H
#define DMZ_V8_QT_UTIL_DOT_H

#include <dmzJsV8UtilTypes.h>
#include <dmzSystemRefCount.h>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <v8.h>

#include <QtCore/QDebug>

namespace dmz {

   V8Value qvariant_to_v8 (const QVariant &Value);
   V8Value qstring_to_v8 (const QString &Value);

   QVariant v8_to_qvariant (V8Value value);
   QString v8_to_qstring (V8Value value);

   struct V8ValueRef : public RefCount {

      V8ValuePersist value;

      QList<V8ValueRef *> &deleteList;

      V8ValueRef (V8Value theValue,  QList<V8ValueRef *> &theList) : deleteList (theList) {

         value = V8ValuePersist::New (theValue);
      }

      virtual ~V8ValueRef () {

         value.Dispose (); value.Clear ();
      }

      virtual void _ref_count_is_zero () {

         deleteList.append (this);
      }
   };


   struct V8Variant {

      V8ValueRef *valueRef;

      V8Variant (V8ValueRef *obj) : valueRef (obj) {

         if (valueRef) { valueRef->ref (); }
      }

      V8Variant (const V8Variant &Type) : valueRef ((V8ValueRef *)Type.valueRef) {

         if (valueRef) { valueRef->ref (); }
      }

      V8Variant &operator= (const V8Variant &Type) {

         if (valueRef) { valueRef->unref (); valueRef = 0; }
         valueRef = (V8ValueRef *)Type.valueRef;
         if (valueRef) { valueRef->ref (); }

         return *this;
      }

      V8Variant () : valueRef (0) {;}

      ~V8Variant () { if (valueRef) { valueRef->unref (); valueRef = 0; } }
   };
};

Q_DECLARE_METATYPE(dmz::V8Variant)


inline dmz::V8Value
dmz::qstring_to_v8 (const QString &Value) {

   return v8::String::New (qPrintable (Value));;
}


inline QString
dmz::v8_to_qstring (V8Value value) {

   return QString (*(v8::String::AsciiValue (value)));
}


#endif // DMZ_V8_QT_OBJECT_DOT_H
