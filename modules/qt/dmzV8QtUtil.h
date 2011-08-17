#ifndef DMZ_V8_QT_UTIL_DOT_H
#define DMZ_V8_QT_UTIL_DOT_H

#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzSystemRefCount.h>
#include <QtCore/QRect>
#include <QtCore/QRectF>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtGui/QKeySequence>
#include <v8.h>

namespace dmz {

   V8Value qvariant_to_v8 (const QVariant &Value);
   V8Value qstring_to_v8 (const QString &Value);
   V8Value qdatetime_to_v8 (const QDateTime &value);
   V8Value qstringlist_to_v8 (const QStringList &Value);
   V8Value qcolor_to_v8 (const QColor &Value);
   V8Value qfont_to_v8 (const QFont &Value);
   V8Value qrectf_to_v8 (const QRectF &Value);
   V8Value qrect_to_v8 (const QRect &Value);
   V8Value qpoint_to_v8 (const QPoint pos);
   V8Value qpointf_to_v8 (const QPointF pos);
   V8Value qsize_to_v8 (const QSize &Value);

   QVariant v8_to_qvariant (V8Value value);
   QString v8_to_qstring (V8Value value);
   QDateTime v8_to_qdatetime (V8Value value);
   QStringList v8_to_qstringlist (V8Value value);
   QColor v8_to_qcolor (V8Value value);
   QFont v8_to_qfont (V8Value value);
   QRectF v8_to_qrectf (V8Value value);
   QRect v8_to_qrect (V8Value value);
   QSize v8_to_qsize (V8Value value);

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

inline QDateTime
dmz::v8_to_qdatetime (V8Value value) {

   QDateTime dt;
   v8::HandleScope scope;
   if (v8_is_valid(value) && value->IsDate ()) {

      v8::Handle<v8::Date> date = v8::Handle<v8::Date>::Cast (value);
      dt = QDateTime::fromTime_t (date->NumberValue () / 1000);
   }
   return dt;
}

inline dmz::V8Value
dmz::qdatetime_to_v8 (const QDateTime &value) {

   v8::HandleScope scope;
   return v8::Date::New ((Float64)value.toTime_t () * 1000.0);
}


#endif // DMZ_V8_QT_OBJECT_DOT_H
