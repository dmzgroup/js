#ifndef DMZ_V8_QT_UTIL_DOT_H
#define DMZ_V8_QT_UTIL_DOT_H

#include <dmzJsV8UtilTypes.h>
#include <dmzSystemRefCount.h>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QKeySequence>
#include <v8.h>

#include <QtCore/QDebug>

namespace dmz {

   V8Value qvariant_to_v8 (const QVariant &Value);
   V8Value qstring_to_v8 (const QString &Value);
   V8Value qstringlist_to_v8 (const QStringList &Value);

   QVariant v8_to_qvariant (V8Value value);
   QString v8_to_qstring (V8Value value);
   QStringList v8_to_qstringlist (V8Value value);

   QKeySequence v8_to_qkeysequence (V8Value value);

   struct V8ValueRef : public RefCount {

      V8ValuePersist value;

      QList<V8ValueRef *> &deleteList;

      V8ValueRef (V8Value theValue,  QList<V8ValueRef *> &theList);
      virtual ~V8ValueRef ();

      virtual void _ref_count_is_zero ();
   };

   struct V8Variant {

      V8ValueRef *valueRef;

      V8Variant ();
      V8Variant (V8ValueRef *obj);
      V8Variant (const V8Variant &Type);

      ~V8Variant ();

      V8Variant &operator= (const V8Variant &Type);
   };
};

Q_DECLARE_METATYPE(dmz::V8Variant)


inline dmz::V8Value
dmz::qstring_to_v8 (const QString &Value) {

   return v8::String::New (qPrintable (Value));
}


inline QString
dmz::v8_to_qstring (V8Value value) {

   return QString (*(v8::String::AsciiValue (value)));
}


#endif // DMZ_V8_QT_OBJECT_DOT_H
