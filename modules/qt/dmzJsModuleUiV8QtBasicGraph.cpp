#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"
#include <QtGui/QFrame>
#include <QtGui/QBrush>
#include <QtGui/QGraphicsLineItem>
#include <QtGui/QGraphicsPathItem>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsTextItem>
#include <QtGui/QGraphicsView>
#include <QtGui/QPen>


namespace {

QMap<QGraphicsItem *, v8::Persistent<v8::Value> > _weakGItemsTable;

dmz::V8Value
qrectf_to_v8 (const QRectF &Value) {

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
v8_to_qrectf (dmz::V8Value value) {

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
qrect_to_v8 (const QRect &Value) {

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
v8_to_qrect (dmz::V8Value value) {

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
qcolor_to_v8 (const QColor &Value) {

   v8::HandleScope scope;
   dmz::V8Object result;

   result = v8::Object::New ();
   result->Set (v8::String::NewSymbol ("r"), v8::Number::New (Value.redF ()));
   result->Set (v8::String::NewSymbol ("g"), v8::Number::New (Value.greenF ()));
   result->Set (v8::String::NewSymbol ("b"), v8::Number::New (Value.blueF ()));

   return scope.Close (result);
}


QColor
v8_to_qcolor (dmz::V8Value value) {

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


void local_recursive_unweaken (QGraphicsItem *curr) {

   if (_weakGItemsTable.contains(curr)) {

      _weakGItemsTable[curr].ClearWeak ();
      _weakGItemsTable.remove (curr);
   }

   QList<QGraphicsItem *> children = curr->childItems ();
   int length = children.count ();
   for (int idx = 0; idx < length; ++idx) {

      local_recursive_unweaken (children.at (idx));
   }
}

void local_gitem_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      QGraphicsItem *ptr = (QGraphicsItem *)param;
      if (!ptr->parentItem ()) { delete ptr; ptr = 0; }
   }

   object.Dispose (); object.Clear ();
}


void local_gbrush_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      QBrush *ptr = (QBrush *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}


void local_gpen_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      QPen *ptr = (QPen *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}


void local_gpainter_path_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      QPainterPath *ptr = (QPainterPath *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}


void local_gpainter_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      QPainter *ptr = (QPainter *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}


void local_gimage_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      QImage *ptr = (QImage *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}


void local_gpixmap_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      QPixmap *ptr = (QPixmap *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}


void local_gpaint_device_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      QPaintDevice *ptr = (QPaintDevice *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}
};


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_grect_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsRectItem *item = 0;
      if (Args.Length () == 1) {

         item = new QGraphicsRectItem (self->_to_graphics_item (Args[0]));
      }
      if (Args.Length () >= 4) {

         QGraphicsItem *parent = 0;
         qreal x, y, w, h;
         x = v8_to_number (Args[0]);
         y = v8_to_number (Args[1]);
         w = v8_to_number (Args[2]);
         h = v8_to_number (Args[3]);
         if (Args.Length () >= 5) { parent = self->_to_graphics_item (Args[4]); }
         item = new QGraphicsRectItem (x, y, w, h, parent);
      }
      result = self->create_v8_graphics_item (item);
   }
   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gtext_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      if (Args.Length ()) {

         QGraphicsTextItem *item (0);
         QGraphicsItem *parent (0);

         if (Args[0]->IsString ()) {

            if (Args.Length () == 2) { parent = self->_to_graphics_item (Args[1]); }
            item = new QGraphicsTextItem (v8_to_qstring (Args[0]), parent);
         }
         else { item = new QGraphicsTextItem (self->_to_graphics_item (Args[0])); }
         result = self->create_v8_graphics_item (item);
      }
   }
   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gline_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      if (Args.Length () >= 4) {

         QGraphicsLineItem *item = 0;
         qreal x1, y1, x2, y2;
         x1 = v8_to_number (Args[0]);
         y1 = v8_to_number (Args[1]);
         x2 = v8_to_number (Args[2]);
         y2 = v8_to_number (Args[3]);

         QGraphicsItem *parent = 0;
         if (Args.Length () >= 5) { parent = self->_to_graphics_item (Args[4]); }

         item = new QGraphicsLineItem (x1, y1, x2, y2, parent);
         result = self->create_v8_graphics_item (item);
      }
   }
   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gpath_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *parent = 0;
      QPainterPath *path = 0;
      QGraphicsPathItem *item = 0;
      if (Args.Length ()) {

         path = self->_to_gpainter_path (Args[0]);
         if (path && (Args.Length () >= 2)) { parent = self->_to_graphics_item (Args[1]); }
         if (!path) { parent = self->_to_graphics_item (Args[0]); }
      }
      if (path) { item = new QGraphicsPathItem (*path, parent); }
      else { item = new QGraphicsPathItem (parent); }
      result = self->create_v8_graphics_item (item);
   }
   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gscene (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QObject *parent = 0;
      QGraphicsScene *scene = 0;
      if (Args.Length () == 1) { parent = self->_to_qobject (Args[0]); }
      if (Args.Length () >= 4) {

         qreal x, y, w, h;
         x = v8_to_number (Args[0]);
         y = v8_to_number (Args[1]);
         w = v8_to_number (Args[2]);
         h = v8_to_number (Args[3]);
         if (Args.Length () >= 5) { parent = self->_to_qobject (Args[4]); }
         scene = new QGraphicsScene (x, y, w, h, parent);
      }
      else { scene = new QGraphicsScene (parent); }
      result = self->create_v8_qobject (scene);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gview (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      QGraphicsScene *scene = 0;
      if (Args.Length ()) {

         parent = self->_to_qwidget (Args[0]);
         if (parent) {

            if (Args.Length () > 1) {

               scene = self->v8_to_qobject<QGraphicsScene> (Args[1]);
            }
         }
         else { scene = self->v8_to_qobject<QGraphicsScene> (Args[0]); }
      }

      QGraphicsView *view = 0;
      if (scene) { view = new QGraphicsView (scene, parent); }
      else { view = new QGraphicsView (parent); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gbrush (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QBrush *brush = 0;
      if (Args.Length ()) {

         QColor result;
         Qt::BrushStyle style = Qt::SolidPattern;
         if (Args.Length ()) { result = v8_to_qcolor (Args[0]); }
         if (Args.Length () > 1) { style = (Qt::BrushStyle) v8_to_uint32 (Args[1]); }
         brush = new QBrush (result, style);
      }
      else { brush = new QBrush (Qt::SolidPattern); }
      result = self->create_v8_gbrush (brush);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gimage (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QImage *image = 0;
      if (Args.Length () == 1) { image = new QImage (v8_to_qstring (Args[0])); }
      if (Args.Length () == 2) {

         if (Args[1]->IsString ()) {

            dmz::String format = v8_to_string (Args[1]);
            image = new QImage (v8_to_qstring (Args[0]), format.get_buffer ());
         }
      }
      if (Args.Length () == 3) {

         int width = v8_to_uint32 (Args[0]);
         int height = v8_to_uint32 (Args[1]);
         QImage::Format format = (QImage::Format)v8_to_uint32 (Args[2]);
         image = new QImage (width, height, format);
      }
      result = self->create_v8_gimage (image);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gpainter (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainter *painter = 0;
      if (Args.Length () == 1) {

         QPaintDevice *pd = self->_to_gpaint_device (Args[0]);
         if (pd) {

            painter = new QPainter (pd);
            result = self->create_v8_gpainter (painter);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gpixmap (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPixmap *pix = 0;
      if (Args.Length () == 1) { pix = new QPixmap (v8_to_qstring (Args[0])); }
      if (Args.Length () == 2) {

         if (Args[1]->IsString ()) {

            dmz::String format = v8_to_string (Args[1]);
            pix = new QPixmap (v8_to_qstring (Args[0]), format.get_buffer ());
         }
         else {

            int width = v8_to_uint32 (Args[0]);
            int height = v8_to_uint32 (Args[1]);
            pix = new QPixmap (width, height);
         }
      }
      result = self->create_v8_gpixmap (pix);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gpen (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPen *pen = 0;
      if (Args.Length () && (Args.Length () < 4)) {

         if (Args.Length () == 1) {

            if (Args[0]->IsNumber ()) {

               pen = new QPen ((Qt::PenStyle)v8_to_uint32 (Args[0]));
            }
            else { pen = new QPen (v8_to_qcolor (Args[0])); }
         }
         else if (Args.Length () == 2) {

            QBrush *brush = self->_to_gbrush (Args[0]);
            qreal w = v8_to_number (Args[1]);
            if (brush && w) { pen = new QPen (*brush, w); }
            else { pen = new QPen (); }
         }
         else if (Args.Length () == 3) {

            qreal r = v8_to_number (Args[0]);
            qreal g = v8_to_number (Args[1]);
            qreal b = v8_to_number (Args[2]);
            pen = new QPen (QColor::fromRgbF (r, g, b));
         }
      }
      else { pen = new QPen (); }
      result = self->create_v8_gpen (pen);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_gpainter_path (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = 0;
      if (Args.Length () == 2) {

         Float32 x, y;
         x = v8_to_number (Args[0]);
         y = v8_to_number (Args[1]);
         path = new QPainterPath (QPointF (x, y));
      }
      else { path = new QPainterPath (); }
      result = self->create_v8_gpainter_path (path);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_gbrush (QBrush *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_gBrushCtor.IsEmpty ()) { obj = _gBrushCtor->NewInstance (); }
      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;

         v8::Persistent<v8::Value> persist = v8::Persistent<v8::Value>::New (result);
         persist.MakeWeak ((void *)value, local_gbrush_delete);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_gpen (QPen *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_gPenCtor.IsEmpty ()) { obj = _gPenCtor->NewInstance (); }
      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;

         v8::Persistent<v8::Value> persist = v8::Persistent<v8::Value>::New (result);
         persist.MakeWeak ((void *)value, local_gpen_delete);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_gpainter_path (QPainterPath *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_gPainterPathCtor.IsEmpty ()) { obj = _gPainterPathCtor->NewInstance (); }
      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;

         v8::Persistent<v8::Value> persist = v8::Persistent<v8::Value>::New (result);
         persist.MakeWeak ((void *)value, local_gpainter_path_delete);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_gpainter (QPainter *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_gPainterCtor.IsEmpty ()) { obj = _gPainterCtor->NewInstance (); }
      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;

         v8::Persistent<v8::Value> persist = v8::Persistent<v8::Value>::New (result);
         persist.MakeWeak ((void *)value, local_gpainter_delete);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_gpixmap (QPixmap *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_gPixmapCtor.IsEmpty ()) { obj = _gPixmapCtor->NewInstance (); }
      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;

         v8::Persistent<v8::Value> persist = v8::Persistent<v8::Value>::New (result);
         persist.MakeWeak ((void *)value, local_gpixmap_delete);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_gpaint_device (QPaintDevice *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_gPaintDeviceCtor.IsEmpty ()) { obj = _gPaintDeviceCtor->NewInstance (); }
      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;

         v8::Persistent<v8::Value> persist = v8::Persistent<v8::Value>::New (result);
         persist.MakeWeak ((void *)value, local_gpaint_device_delete);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_gimage (QImage *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_gImageCtor.IsEmpty ()) { obj = _gImageCtor->NewInstance (); }
      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;

         v8::Persistent<v8::Value> persist = v8::Persistent<v8::Value>::New (result);
         persist.MakeWeak ((void *)value, local_gimage_delete);
      }
   }

   return scope.Close (result);
}

dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_graphics_item (QGraphicsItem *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (qgraphicsitem_cast<QGraphicsRectItem *> (value)) {

         if (!_gRectCtor.IsEmpty ()) { obj = _gRectCtor->NewInstance (); }
         if (!obj.IsEmpty ()) {


            obj->SetInternalField (0, v8::External::Wrap ((void *)value));
            result = obj;
         }
      }
      else if (qgraphicsitem_cast<QGraphicsTextItem *> (value)) {

         if (!_gTextCtor.IsEmpty ()) { obj = _gTextCtor->NewInstance (); }
         if (!obj.IsEmpty ()) {

            obj->SetInternalField (0, v8::External::Wrap ((void *)value));
            result = obj;
         }
      }
      else if (qgraphicsitem_cast<QGraphicsLineItem *> (value)) {

         if (!_gLineCtor.IsEmpty ()) { obj = _gLineCtor->NewInstance (); }
         if (!obj.IsEmpty ()) {

            obj->SetInternalField (0, v8::External::Wrap ((void *)value));
            result = obj;
         }
      }
      else if (qgraphicsitem_cast<QGraphicsPathItem *> (value)) {

         if (!_gPathCtor.IsEmpty ()) { obj = _gPathCtor->NewInstance (); }
         if (!obj.IsEmpty ()) {

            obj->SetInternalField (0, v8::External::Wrap ((void *)value));
            result = obj;
         }
      }
   }

   return scope.Close (result);
}


QGraphicsItem *
dmz::JsModuleUiV8QtBasic::_to_graphics_item (V8Value value) {

   v8::HandleScope scope;
   QGraphicsItem *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_gRectTemp->HasInstance (obj) || _gTextTemp->HasInstance (obj) ||
          _gLineTemp->HasInstance (obj) || _gPathTemp->HasInstance (obj)) {

         result = (QGraphicsItem *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


QPen *
dmz::JsModuleUiV8QtBasic::_to_gpen (V8Value value) {

   v8::HandleScope scope;
   QPen *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_gPenTemp->HasInstance (obj)) {

         result = (QPen *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


QBrush *
dmz::JsModuleUiV8QtBasic::_to_gbrush (V8Value value) {

   v8::HandleScope scope;
   QBrush *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_gBrushTemp->HasInstance (obj)) {

         result = (QBrush *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


QPainterPath *
dmz::JsModuleUiV8QtBasic::_to_gpainter_path (V8Value value) {

   v8::HandleScope scope;
   QPainterPath *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_gPainterPathTemp->HasInstance (obj)) {

         result = (QPainterPath *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


QPainter *
dmz::JsModuleUiV8QtBasic::_to_gpainter (V8Value value) {

   v8::HandleScope scope;
   QPainter *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_gPainterTemp->HasInstance (obj)) {

         result = (QPainter *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


QPixmap *
dmz::JsModuleUiV8QtBasic::_to_gpixmap (V8Value value) {

   v8::HandleScope scope;
   QPixmap *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_gPixmapTemp->HasInstance (obj)) {

         result = (QPixmap *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


QImage *
dmz::JsModuleUiV8QtBasic::_to_gimage (V8Value value) {

   v8::HandleScope scope;
   QImage *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_gImageTemp->HasInstance (obj)) {

         result = (QImage *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


QPaintDevice *
dmz::JsModuleUiV8QtBasic::_to_gpaint_device (V8Value value) {

   v8::HandleScope scope;
   QPaintDevice *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_gPaintDeviceTemp->HasInstance (obj)) {

         result = (QPaintDevice *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_drops (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length () && Args[0]->IsBoolean ()) {

            item->setAcceptDrops (v8_to_boolean (Args[0]));
         }
         result = v8::Boolean::New (item->acceptDrops ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_brect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) { result = qrectf_to_v8 (item->boundingRect ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_child_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         QList<QGraphicsItem *> items = item->childItems ();
         const int Length = items.count ();
         V8Array array = v8::Array::New (Length);
         for (int ix = 0; ix < Length; ++ix) {

            V8Value value = self->create_v8_graphics_item (items.at (ix));
            array->Set (v8::Integer::New (ix), value);
         }
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_children_brect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) { result = qrectf_to_v8 (item->childrenBoundingRect ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_clear_focus (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) { item->clearFocus (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_collides_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::False ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QGraphicsItem *other = self->_to_graphics_item (Args[0]);
            if (other) { result = v8::Boolean::New (item->collidesWithItem (other)); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_collides_path (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QPainterPath *path = self->_to_gpainter_path (Args[0]);
            if (path) { result = v8::Boolean::New (item->collidesWithPath (*path)); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_collides_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         QList<QGraphicsItem *> items = item->collidingItems ();
         const int Length = items.count ();
         V8Array array = v8::Array::New (Length);
         for (int ix = 0; ix < Length; ++ix) {

            V8Value value = self->create_v8_graphics_item (items.at (ix));
            array->Set (v8::Integer::New (ix), value);
         }
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_contains (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length () >= 2) {

            qreal x = v8_to_number (Args[0]);
            qreal y = v8_to_number (Args[1]);
            result = v8::Boolean::New (item->contains (QPointF (x, y)));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_data (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            int index = 0;
            index = v8_to_uint32 (Args[0]);
            if (Args.Length () > 0) {

               QVariant var = v8_to_qvariant (Args[1]);
               item->setData (index, var);
            }
            result = qvariant_to_v8 (item->data (index));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_ensure_vis (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length () >= 4) {

            qreal x, y, w, h;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);

            int xmargin = 50, ymargin = 50;
            if (Args.Length () >= 5) { xmargin = v8_to_uint32 (Args[4]); }
            if (Args.Length () >= 6) { ymargin = v8_to_uint32 (Args[5]); }
            item->ensureVisible (x, y, w, h, xmargin, ymargin);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_focus_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         QGraphicsItem *focus = item->focusItem ();
         if (focus) { result = self->create_v8_graphics_item (focus); }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_has_focus (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::False ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) { result = v8::Boolean::New (item->hasFocus ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_hide (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) { item->hide (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_show (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) { item->show (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_active (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setActive (v8_to_boolean (Args[0])); }
         result = v8::Boolean::New (item->isActive ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_visible (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setVisible (v8_to_boolean (Args[0])); }
         result = v8::Boolean::New (item->isVisible ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_parent_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QGraphicsItem *parent = self->_to_graphics_item (Args[0]);
            if (item) { item->setParentItem (parent); }
         }
         result = self->create_v8_graphics_item (item->parentItem ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_z (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setZValue (v8_to_number (Args[0])); }
         result = v8::Number::New (item->zValue ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_pos (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length () >= 2) {

            qreal x, y;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            item->setPos (x, y);
         }

         QPointF p = item->pos ();
         V8Array array = v8::Array::New (2);
         array->Set (v8::Integer::New (0), v8::Number::New (p.x ()));
         array->Set (v8::Integer::New (1), v8::Number::New (p.y ()));
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_spos (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length () >= 2) {

            qreal x, y;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            item->setPos (x, y);
         }

         QPointF p = item->scenePos ();
         V8Array array = v8::Array::New (2);
         array->Set (v8::Integer::New (0), v8::Number::New (p.x ()));
         array->Set (v8::Integer::New (1), v8::Number::New (p.y ()));
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_stack_before (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QGraphicsItem *sibling = self->_to_graphics_item (Args[0]);
            item->stackBefore (sibling);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_enabled (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setEnabled (v8_to_boolean (Args[0])); }
         result = v8::Boolean::New (item->isEnabled ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_rotation (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setRotation (v8_to_number (Args[0]));}
         result = v8::Number::New (item->rotation ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gitem_flag (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsItem *item = self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QGraphicsItem::GraphicsItemFlag flag =
               (QGraphicsItem::GraphicsItemFlag)v8_to_int32 (Args[0]);
            if (Args.Length () == 2) { item->setFlag (flag, v8_to_boolean(Args[1])); }
            result = v8::Boolean::New (item->flags () | flag);
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_graph () {

   v8::HandleScope scope;

   _graphTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _graphTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _graphTemp->PrototypeTemplate ();
   proto->Set ("acceptDrops", v8::FunctionTemplate::New (_gitem_drops, _self));
   proto->Set ("active", v8::FunctionTemplate::New (_gitem_active, _self));
   proto->Set ("boundingRect", v8::FunctionTemplate::New (_gitem_brect, _self));
   proto->Set ("childItems", v8::FunctionTemplate::New (_gitem_child_items, _self));
   proto->Set ("childrenBoundingRect", v8::FunctionTemplate::New (_gitem_children_brect, _self));
   proto->Set ("clearFocus", v8::FunctionTemplate::New (_gitem_clear_focus, _self));
   proto->Set ("collidesWithItem", v8::FunctionTemplate::New (_gitem_collides_item, _self));
   proto->Set ("collidesWithPath", v8::FunctionTemplate::New (_gitem_collides_path, _self));
   proto->Set ("collidingItems", v8::FunctionTemplate::New (_gitem_collides_items, _self));
   proto->Set ("contains", v8::FunctionTemplate::New (_gitem_contains, _self));
   proto->Set ("data", v8::FunctionTemplate::New (_gitem_data, _self));
   proto->Set ("enabled", v8::FunctionTemplate::New (_gitem_enabled, _self));
   proto->Set ("ensureVisible", v8::FunctionTemplate::New (_gitem_ensure_vis, _self));
   proto->Set ("focusItem", v8::FunctionTemplate::New (_gitem_focus_item, _self));
   proto->Set ("hasFocus", v8::FunctionTemplate::New (_gitem_has_focus, _self));
   proto->Set ("hide", v8::FunctionTemplate::New (_gitem_hide, _self));
   proto->Set ("parentItem", v8::FunctionTemplate::New (_gitem_parent_item, _self));
   proto->Set ("show", v8::FunctionTemplate::New (_gitem_show, _self));
   proto->Set ("stackBefore", v8::FunctionTemplate::New (_gitem_stack_before, _self));
   proto->Set ("visible", v8::FunctionTemplate::New (_gitem_visible, _self));
   proto->Set ("z", v8::FunctionTemplate::New (_gitem_z, _self));
   proto->Set ("pos", v8::FunctionTemplate::New (_gitem_pos, _self));
   proto->Set ("scenePos", v8::FunctionTemplate::New (_gitem_spos, _self));
   proto->Set ("rotation", v8::FunctionTemplate::New (_gitem_rotation, _self));
   proto->Set ("flag", v8::FunctionTemplate::New (_gitem_flag, _self));

   _graphApi.add_constant ("ItemIsMovable", (UInt32)QGraphicsItem::ItemIsMovable);
   _graphApi.add_constant ("ItemIsSelectable", (UInt32)QGraphicsItem::ItemIsSelectable);
   _graphApi.add_constant ("ItemIsFocusable", (UInt32)QGraphicsItem::ItemIsFocusable);
   _graphApi.add_constant ("ItemClipsToShape", (UInt32)QGraphicsItem::ItemClipsToShape);
   _graphApi.add_constant ("ItemClipsChildrenToShape", (UInt32)QGraphicsItem::ItemClipsChildrenToShape);
   _graphApi.add_constant ("ItemIgnoresTransformations", (UInt32)QGraphicsItem::ItemIgnoresTransformations);
   _graphApi.add_constant ("ItemIgnoresParentOpacity", (UInt32)QGraphicsItem::ItemIgnoresParentOpacity);
   _graphApi.add_constant ("ItemDoesntPropagateOpacityToChildren", (UInt32)QGraphicsItem::ItemDoesntPropagateOpacityToChildren);
   _graphApi.add_constant ("ItemStacksBehindParent", (UInt32)QGraphicsItem::ItemStacksBehindParent);
   _graphApi.add_constant ("ItemUsesExtendedStyleOption", (UInt32)QGraphicsItem::ItemUsesExtendedStyleOption);
   _graphApi.add_constant ("ItemHasNoContents", (UInt32)QGraphicsItem::ItemHasNoContents);
   _graphApi.add_constant ("ItemSendsGeometryChanges", (UInt32)QGraphicsItem::ItemSendsGeometryChanges);
   _graphApi.add_constant ("ItemAcceptsInputMethod", (UInt32)QGraphicsItem::ItemAcceptsInputMethod);
   _graphApi.add_constant ("ItemNegativeZStacksBehindParent", (UInt32)QGraphicsItem::ItemNegativeZStacksBehindParent);
   _graphApi.add_constant ("ItemIsPanel", (UInt32)QGraphicsItem::ItemIsPanel);
   _graphApi.add_constant ("ItemSendsScenePositionChanges", (UInt32)QGraphicsItem::ItemSendsScenePositionChanges);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gabs_brush (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAbstractGraphicsShapeItem *item =
         (QAbstractGraphicsShapeItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QBrush *brush = self->_to_gbrush (Args[0]);
            if (brush) { item->setBrush (*brush); }
         }
         QBrush *brush = new QBrush(item->brush ());
         result = self->create_v8_gbrush (brush);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gabs_pen (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAbstractGraphicsShapeItem *item =
         (QAbstractGraphicsShapeItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QPen *pen = self->_to_gpen (Args[0]);
            if (pen) { item->setPen (*pen); }
         }
         result = self->create_v8_gpen (new QPen(item->pen ()));
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_abs_graph_item () {

   v8::HandleScope scope;

   _gAbsItemTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _gAbsItemTemp->Inherit (_graphTemp);

   V8ObjectTemplate instance = _gAbsItemTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gAbsItemTemp->PrototypeTemplate ();
   proto->Set ("brush", v8::FunctionTemplate::New (_gabs_brush, _self));
   proto->Set ("pen", v8::FunctionTemplate::New (_gabs_pen, _self));
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_grect_rect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsRectItem *item =
         (QGraphicsRectItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length () == 1) { item->setRect (v8_to_qrectf (Args[0])); }
         if (Args.Length () >= 4) {

            qreal x, y, w, h;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);
            item->setRect (x, y, w, h);
         }
         result = qrectf_to_v8 (item->rect ());
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_grect_item () {

   v8::HandleScope scope;

   _gRectTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _gRectTemp->Inherit (_gAbsItemTemp);

   V8ObjectTemplate instance = _gRectTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gRectTemp->PrototypeTemplate ();
   proto->Set ("rect",  v8::FunctionTemplate::New (_grect_rect, _self));

   _graphApi.add_function ("createRectItem", _create_grect_item, _self);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpath_path (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsPathItem *item =
         (QGraphicsPathItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QPainterPath *path = self->_to_gpainter_path (Args[0]);
            if (path) { item->setPath (*path); }
         }
         result = self->create_v8_gpainter_path (new QPainterPath (item->path ()));
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gpath_item () {

   v8::HandleScope scope;

   _gPathTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _gPathTemp->Inherit (_gAbsItemTemp);

   V8ObjectTemplate instance = _gPathTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gPathTemp->PrototypeTemplate ();
   proto->Set ("path",  v8::FunctionTemplate::New (_gpath_path, _self));

   _graphApi.add_function ("createPathItem", _create_gpath_item, _self);

}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gtext_adjust_size (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsTextItem *item =
         (QGraphicsTextItem *)self->_to_graphics_item (Args.This ());
      if (item) { item->adjustSize (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gtext_text_color (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsTextItem *item =
         (QGraphicsTextItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length () == 1) { item->setDefaultTextColor (v8_to_qcolor (Args[0])); }
         else if (Args.Length () >= 3) {

            qreal r = v8_to_number (Args[0]);
            qreal g = v8_to_number (Args[1]);
            qreal b = v8_to_number (Args[2]);
            item->setDefaultTextColor (QColor::fromRgbF (r, g, b));
         }
         result = qcolor_to_v8 (item->defaultTextColor ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gtext_plain_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsTextItem *item =
         (QGraphicsTextItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setPlainText (v8_to_qstring (Args[0])); }
         result = qstring_to_v8 (item->toPlainText ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gtext_html_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsTextItem *item =
         (QGraphicsTextItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setHtml (v8_to_qstring (Args[0])); }
         result = qstring_to_v8 (item->toHtml ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gtext_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsTextItem *item =
         (QGraphicsTextItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setTextWidth (v8_to_number (Args[0])); }
         result = v8::Number::New (item->textWidth ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gtext_text_interact_flags (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsTextItem *item =
         (QGraphicsTextItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            item->setTextInteractionFlags (
               (Qt::TextInteractionFlag)v8_to_uint32 (Args[0]));
         }
         result = v8::Number::New (item->textInteractionFlags ());
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gtext_item () {

   v8::HandleScope scope;

   _gTextTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _gTextTemp->Inherit (_graphTemp);

   V8ObjectTemplate instance = _gTextTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gTextTemp->PrototypeTemplate ();

   proto->Set ("adjustSize", v8::FunctionTemplate::New (_gtext_adjust_size, _self));
   proto->Set ("textColor", v8::FunctionTemplate::New (_gtext_text_color, _self));
   proto->Set ("plainText", v8::FunctionTemplate::New (_gtext_plain_text, _self));
   proto->Set ("htmlText", v8::FunctionTemplate::New (_gtext_html_text, _self));
   proto->Set ("width", v8::FunctionTemplate::New (_gtext_width, _self));
   proto->Set (
      "textInteractionFlags",
      v8::FunctionTemplate::New (_gtext_text_interact_flags, _self));

   _graphApi.add_function ("createTextItem", _create_gtext_item, _self);
   _graphApi.add_constant ("NoTextInteraction", (UInt32)Qt::NoTextInteraction);
   _graphApi.add_constant ("TextSelectableByMouse", (UInt32)Qt::TextSelectableByMouse);
   _graphApi.add_constant (
      "TextSelectableByKeyboard",
      (UInt32)Qt::TextSelectableByKeyboard);
   _graphApi.add_constant ("LinksAccessibleByMouse", (UInt32)Qt::LinksAccessibleByMouse);
   _graphApi.add_constant (
      "LinksAccessibleByKeyboard",
      (UInt32)Qt::LinksAccessibleByKeyboard);
   _graphApi.add_constant ("TextEditable", (UInt32)Qt::TextEditable);
   _graphApi.add_constant ("TextEditorInteraction", (UInt32)Qt::TextEditorInteraction);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gline_line (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsLineItem *item =
         (QGraphicsLineItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         qreal x1, y1, x2, y2;
         if ((Args.Length () == 1) && (Args[0]->IsObject ())) {

            V8Object obj = v8_to_object (Args[0]);
            x1 = v8_to_number (obj->Get (v8::String::NewSymbol ("x1")));
            y1 = v8_to_number (obj->Get (v8::String::NewSymbol ("y1")));
            x2 = v8_to_number (obj->Get (v8::String::NewSymbol ("x2")));
            y2 = v8_to_number (obj->Get (v8::String::NewSymbol ("y2")));
            item->setLine (x1, y1, x2, y2);
         }
         else if (Args.Length () == 4) {

            x1 = v8_to_number (Args[0]);
            y1 = v8_to_number (Args[1]);
            x2 = v8_to_number (Args[2]);
            y2 = v8_to_number (Args[3]);
            item->setLine (x1, y1, x2, y2);
         }
         QLineF line = item->line ();
         V8Object obj = v8::Object::New ();
         obj->Set (v8::String::NewSymbol ("x1"), v8::Number::New (line.x1 ()));
         obj->Set (v8::String::NewSymbol ("y1"), v8::Number::New (line.y1 ()));
         obj->Set (v8::String::NewSymbol ("x2"), v8::Number::New (line.x2 ()));
         obj->Set (v8::String::NewSymbol ("y2"), v8::Number::New (line.y2 ()));

         result = obj;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gline_pen (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsLineItem *item =
         (QGraphicsLineItem *)self->_to_graphics_item (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QPen *pen = self->_to_gpen (Args[0]);
            if (pen) { item->setPen (*pen); }
         }
         result = self->create_v8_gpen (new QPen (item->pen ()));
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gline_item () {

   v8::HandleScope scope;

   _gLineTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _gLineTemp->Inherit (_graphTemp);

   V8ObjectTemplate instance = _gLineTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gLineTemp->PrototypeTemplate ();
   proto->Set ("line", v8::FunctionTemplate::New (_gline_line, _self));
   proto->Set ("pen", v8::FunctionTemplate::New (_gline_pen, _self));

   _graphApi.add_function ("createLineItem", _create_gline_item, _self);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_alignment (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length ()) {

            Qt::Alignment align = (Qt::Alignment)v8_to_uint32 (Args[0]);
            view->setAlignment (align);
         }
         result = v8::Number::New ((UInt32)view->alignment ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_bg_brush (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length ()) {

            QBrush *brush = self->_to_gbrush (Args[0]);
            if (brush) { view->setBackgroundBrush (*brush); }
         }
         result = self->create_v8_gbrush (new QBrush (view->backgroundBrush ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_cache (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::False ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length ()) {

            view->setCacheMode (
                  (QGraphicsView::CacheMode)(v8_to_boolean (Args[0]) ?
                              QGraphicsView::CacheBackground :
                              QGraphicsView::CacheNone));
         }
         result = v8::Boolean::New (
               (view->cacheMode () == QGraphicsView::CacheBackground) ? true : false);

      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_center (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length ()) {

            if (Args.Length () == 1) {

               QGraphicsItem *item = self->_to_graphics_item (Args[0]);
               if (item) { view->centerOn (item); }
            }
            else {

               qreal x, y;
               x = v8_to_number (Args[0]);
               y = v8_to_number (Args[1]);
               view->centerOn (x, y);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_visible (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         QGraphicsItem *item = 0;
         int xmargin, ymargin;
         xmargin = ymargin = 50;

         switch (Args.Length ()) {

            case 6: ymargin = v8_to_uint32 (Args[5]);
            case 5: xmargin = v8_to_uint32 (Args[4]);
            case 4:
               qreal x, y, w, h;
               x = v8_to_number (Args[0]);
               y = v8_to_number (Args[1]);
               w = v8_to_number (Args[2]);
               h = v8_to_number (Args[3]);
               view->ensureVisible (x, y, w, h, xmargin, ymargin);
               break;
            case 3: ymargin = v8_to_uint32 (Args[2]);
            case 2: xmargin = v8_to_uint32 (Args[1]);
            case 1:
               item = self->_to_graphics_item (Args[0]);
               if (item) { view->ensureVisible (item, xmargin, ymargin); }
               break;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_fg_brush (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length ()) {

            QBrush *brush = self->_to_gbrush (Args[0]);
            if (brush) { view->setForegroundBrush (*brush); }
         }
         result = self->create_v8_gbrush (new QBrush (view->foregroundBrush ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_interactive (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length ()) {

            view->setInteractive (v8_to_boolean (Args[0]));
         }
         result = v8::Boolean::New (view->isInteractive ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_item_at (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length () >= 2) {

            int x, y;
            x = v8_to_uint32 (Args[0]);
            y = v8_to_uint32 (Args[1]);
            QGraphicsItem *item = view->itemAt (x, y);
            if (item) { result = self->create_v8_graphics_item (item); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         QList<QGraphicsItem *> list;
         if (Args.Length ()) {

            QPainterPath *path (0);
            path = self->_to_gpainter_path (Args[0]);
            if (path) { list = view->items (*path); }
            else if (Args.Length () == 2) {

               list = view->items (v8_to_uint32 (Args[0]), v8_to_uint32 (Args[1]));
            }
            else if (Args.Length () == 4) {

               int x, y, w, h;
               x = v8_to_uint32 (Args[0]);
               y = v8_to_uint32 (Args[1]);
               w = v8_to_uint32 (Args[2]);
               h = v8_to_uint32 (Args[3]);
               list = view->items (x, y, w, h);
            }
         }
         else { list = view->items (); }

         int length = list.length ();
         V8Array array = v8::Array::New (length);
         for (UInt32 idx = 0; idx < length; ++idx) {

            array->Set (
               v8::Integer::New (idx),
               self->create_v8_graphics_item (list[idx]));
         }
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_rotate (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view && Args.Length ()) { view->rotate (v8_to_number (Args[0])); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_scale (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length () == 2) {

            view->scale (v8_to_number (Args[0]), v8_to_number (Args[1]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_scene (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length ()) {

            QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args[0]);
            if (scene) { view->setScene (scene); }
         }
         result = self->create_v8_qobject (view->scene ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_scene_rect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         qreal x, y, w, h;
         if ((Args.Length () == 1) && Args[0]->IsObject ()) {

            view->setSceneRect (v8_to_qrectf (Args[0]));
         }
         if (Args.Length () == 4) {

            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);
            view->setSceneRect (QRectF (x, y, w, h));

         }
         result = qrectf_to_v8 (view->sceneRect ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_viewport_update_mode (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length ()) {

            view->setViewportUpdateMode (
               (QGraphicsView::ViewportUpdateMode)v8_to_uint32 (Args[0]));
         }
         result = v8::Number::New (view->viewportUpdateMode ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_translate (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length () == 2) {

            qreal dx, dy;
            dx = v8_to_number (Args[0]);
            dy = v8_to_number (Args[1]);
            view->translate (dx, dy);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_viewport (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) { result = self->create_v8_qwidget (view->viewport ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gview_render (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsView *view = self->v8_to_qobject<QGraphicsView> (Args.This ());
      if (view) {

         if (Args.Length ()) {

            QPainter *painter = self->_to_gpainter (Args[0]);
            QRect target, source;
            if (Args.Length () > 1) { target = v8_to_qrect (Args[1]); }
            if (Args.Length () > 2) { source = v8_to_qrect (Args[2]); }
            view->render (painter, target, source);
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gview () {

   v8::HandleScope scope;

   _gViewTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _gViewTemp->Inherit (_frameTemp);

   V8ObjectTemplate instance = _gViewTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gViewTemp->PrototypeTemplate ();

   proto->Set ("alignment", v8::FunctionTemplate::New (_gview_alignment, _self));
   proto->Set ("backgroundBrush", v8::FunctionTemplate::New (_gview_bg_brush, _self));
   proto->Set ("cache", v8::FunctionTemplate::New (_gview_cache, _self));
   proto->Set ("centerOn", v8::FunctionTemplate::New (_gview_center, _self));
   proto->Set ("ensureVisible", v8::FunctionTemplate::New (_gview_visible, _self));
   proto->Set ("foregroundBrush", v8::FunctionTemplate::New (_gview_fg_brush, _self));
   proto->Set ("interactive", v8::FunctionTemplate::New (_gview_interactive, _self));
   proto->Set ("itemAt", v8::FunctionTemplate::New (_gview_item_at, _self));
   proto->Set ("items", v8::FunctionTemplate::New (_gview_items, _self));
   proto->Set ("rotate", v8::FunctionTemplate::New (_gview_rotate, _self));
   proto->Set ("scale", v8::FunctionTemplate::New (_gview_scale, _self));
   proto->Set ("scene", v8::FunctionTemplate::New (_gview_scene, _self));
   proto->Set ("sceneRect", v8::FunctionTemplate::New (_gview_scene_rect, _self));
   proto->Set ("render", v8::FunctionTemplate::New (_gview_render, _self));
   proto->Set ("viewport", v8::FunctionTemplate::New (_gview_viewport, _self));
   proto->Set (
      "viewportUpdateMode",
      v8::FunctionTemplate::New (_gview_viewport_update_mode, _self));
   proto->Set ("translate", v8::FunctionTemplate::New (_gview_translate, _self));

   _graphApi.add_function ("createView", _create_gview, _self);
   _graphApi.add_constant (
      "FullViewportUpdate",
      (UInt32)QGraphicsView::FullViewportUpdate);
   _graphApi.add_constant (
      "MinimalViewportUpdate",
      (UInt32)QGraphicsView::MinimalViewportUpdate);
   _graphApi.add_constant (
      "SmartViewportUpdate",
      (UInt32)QGraphicsView::BoundingRectViewportUpdate);
   _graphApi.add_constant (
      "BoundingRectViewportUpdate",
      (UInt32)QGraphicsView::BoundingRectViewportUpdate);

}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_active_panel (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length ()) {

            QGraphicsItem *item = self->_to_graphics_item (Args[0]);
            if (item) { scene->setActivePanel (item); }
         }

         QGraphicsItem *item = scene->activePanel ();
         if (item) { result = self->create_v8_graphics_item (item); }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_add_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length ()) {

            QGraphicsItem *item = self->_to_graphics_item (Args[0]);
            if (item) {

               local_recursive_unweaken (item);
               scene->addItem (item);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_add_line (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length () == 1) {

            QGraphicsItem *item = self->_to_graphics_item (Args[0]);
            if (item) { scene->addItem (item); }
            result = Args[0];
         }
         if (Args.Length () >= 4) {

            QGraphicsLineItem *item (0);
            qreal x1, x2, y1, y2;
            x1 = v8_to_number (Args[0]);
            y1 = v8_to_number (Args[1]);
            x2 = v8_to_number (Args[2]);
            y2 = v8_to_number (Args[3]);

            if (Args.Length () > 4) {

               QPen *pen = self->_to_gpen (Args[4]);
               if (pen) { item = scene->addLine (x1, y1, x2, y2, *pen); }
            }
            else { item = scene->addLine (x1, y1, x2, y2); }

            if (item) { result = self->create_v8_graphics_item (item); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_add_path (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length ()) {

            QGraphicsPathItem *item (0);
            QPainterPath *path = self->_to_gpainter_path (Args[0]);
            QPen *pen (0);
            QBrush *brush (0);
            if (Args.Length () > 1) { pen = self->_to_gpen (Args[1]); }
            if (Args.Length () > 2) { brush = self->_to_gbrush (Args[2]); }
            if (path) {

               item =
                  scene->addPath (
                     *path,
                     pen ? *pen : QPen (),
                     brush ? *brush : QBrush ());
            }

            if (item) { result = self->create_v8_graphics_item (item); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_add_rect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length () == 1) {

            QGraphicsItem *item = self->_to_graphics_item (Args[0]);
            if (item) { scene->addItem (item); }
            result = Args[0];
         }
         if (Args.Length () >= 4) {

            QGraphicsRectItem *item (0);
            qreal x, y, w, h;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);
            QPen *pen (0);
            QBrush *brush (0);
            if (Args.Length () >= 5) { pen = self->_to_gpen (Args[4]); }
            if (Args.Length () >= 6) { brush = self->_to_gbrush (Args[5]); }
            item = scene->addRect (
               x,
               y,
               w,
               h,
               pen ? *pen : QPen (),
               brush ? *brush : QBrush ());

            if (item) { result = self->create_v8_graphics_item (item); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_add_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length ()) {

            QGraphicsTextItem *item (0);
            if (Args[0]->IsString ()) {

               item = scene->addText (v8_to_qstring (Args[0]));
               if (item) { result = self->create_v8_graphics_item (item); }
            }
            else {

               item =
                  qgraphicsitem_cast<QGraphicsTextItem *>(
                     self->_to_graphics_item (Args[0]));

               if (item) {

                  scene->addItem (item);
                  result = Args[0];
               }
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_bg_brush (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length ()) {

            QBrush *brush = self->_to_gbrush (Args[0]);
            if (brush) { scene->setBackgroundBrush (*brush); }
         }
         result = self->create_v8_gbrush (new QBrush (scene->backgroundBrush ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_clear_focus (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) { scene->clearFocus (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_colliding_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length ()) {

            QGraphicsItem *item = self->_to_graphics_item (Args[0]);
            QList<QGraphicsItem *> items = scene->collidingItems (item);
            const int Length = items.count ();
            V8Array array = v8::Array::New (Length);
            for (int ix = 0; ix < Length; ++ix) {

               V8Value value = self->create_v8_graphics_item (items.at (ix));
               array->Set (v8::Integer::New (ix), value);
            }
            result = array;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_fg_brush (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length ()) {

            QBrush *brush = self->_to_gbrush (Args[0]);
            if (brush) { scene->setForegroundBrush (*brush); }
         }
         result = self->create_v8_gbrush (new QBrush (scene->foregroundBrush ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_focus_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length ()) {

            QGraphicsItem *item = self->_to_graphics_item (Args[0]);
            scene->setFocusItem (item);
         }
         QGraphicsItem *item = scene->focusItem ();
         result = self->create_v8_graphics_item (item);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_height (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) { result = v8::Number::New (scene->height ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         QList<QGraphicsItem *> list;
         if (Args.Length () == 0) { list = scene->items (); }
         else if (Args.Length () == 1) {

            Qt::SortOrder sort = (Qt::SortOrder) v8_to_uint32 (Args[0]);
            list = scene->items (sort);
         }
         else if (Args.Length () == 3) {

            QPainterPath *path = self->_to_gpainter_path (Args[0]);
            Qt::ItemSelectionMode ism = (Qt::ItemSelectionMode)v8_to_uint32 (Args[1]);
            Qt::SortOrder sort = (Qt::SortOrder) v8_to_uint32 (Args[2]);

            if (path) { list = scene->items (*path, ism, sort); }
         }
         else if (Args.Length () == 4) {

            qreal x, y;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);

            Qt::ItemSelectionMode ism = (Qt::ItemSelectionMode)v8_to_uint32 (Args[2]);
            Qt::SortOrder sort = (Qt::SortOrder) v8_to_uint32 (Args[3]);
            list = scene->items (QPointF (x, y), ism, sort);
         }
         else if (Args.Length () == 6) {

            qreal x, y, w, h;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);

            Qt::ItemSelectionMode ism = (Qt::ItemSelectionMode)v8_to_uint32 (Args[4]);
            Qt::SortOrder sort = (Qt::SortOrder) v8_to_uint32 (Args[5]);
            list = scene->items (x, y, w, h, ism, sort);
         }

         if (!list.isEmpty ()) {

            const int Length = list.count ();
            V8Array array = v8::Array::New (Length);
            for (int ix = 0; ix < Length; ++ix) {

               V8Value value = self->create_v8_graphics_item (list.at (ix));
               array->Set (v8::Integer::New (ix), value);
            }
            result = array;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_remove_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if (Args.Length ()) {

            QGraphicsItem *item = self->_to_graphics_item (Args[0]);
            if (item) {

               scene->removeItem (item);
               v8::Persistent<v8::Value> persist = v8::Persistent<v8::Value>::New (Args[0]);
               persist.MakeWeak ((void *)item, local_gitem_delete);
               _weakGItemsTable.insert (item, persist);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_scene_rect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         if ((Args.Length () == 1) && Args[0]->IsObject ()) {

            scene->setSceneRect (v8_to_qrectf (Args[0]));
         }
         if (Args.Length () >= 4) {

            qreal x, y, w, h;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);
            scene->setSceneRect (x, y, w, h);
         }
         result = qrectf_to_v8 (scene->sceneRect ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_selected_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) {

         QList<QGraphicsItem *> list = scene->selectedItems ();
         const int Length = list.count ();
         V8Array array = v8::Array::New (Length);
         for (int ix = 0; ix < Length; ++ix) {

            V8Value value = self->create_v8_graphics_item (list.at (ix));
            array->Set (v8::Integer::New (ix), value);
         }
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gscene_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGraphicsScene *scene = self->v8_to_qobject<QGraphicsScene> (Args.This ());
      if (scene) { result = v8::Number::New (scene->width ()); }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gscene () {

   v8::HandleScope scope;

   _gSceneTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _gSceneTemp->Inherit (_objectTemp);

   V8ObjectTemplate instance = _gSceneTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gSceneTemp->PrototypeTemplate ();
   proto->Set ("addItem", v8::FunctionTemplate::New (_gscene_add_item, _self));
   proto->Set ("addLine", v8::FunctionTemplate::New (_gscene_add_line, _self));
   proto->Set ("addPath", v8::FunctionTemplate::New (_gscene_add_path, _self));
   proto->Set ("addRect", v8::FunctionTemplate::New (_gscene_add_rect, _self));
   proto->Set ("addText", v8::FunctionTemplate::New (_gscene_add_text, _self));
   proto->Set ("backgroundBrush", v8::FunctionTemplate::New (_gscene_bg_brush, _self));
   proto->Set ("clearFocus", v8::FunctionTemplate::New (_gscene_clear_focus, _self));
   proto->Set ("collidingItems", v8::FunctionTemplate::New (_gscene_colliding_items, _self));
   proto->Set ("focusItem", v8::FunctionTemplate::New (_gscene_focus_item, _self));
   proto->Set ("foregroundBrush", v8::FunctionTemplate::New (_gscene_fg_brush, _self));
   proto->Set ("activePanel", v8::FunctionTemplate::New (_gscene_active_panel, _self));
   proto->Set ("items", v8::FunctionTemplate::New (_gscene_items, _self));
   proto->Set ("removeItem", v8::FunctionTemplate::New (_gscene_remove_item, _self));
   proto->Set ("sceneRect", v8::FunctionTemplate::New (_gscene_scene_rect, _self));
   proto->Set ("selectedItems", v8::FunctionTemplate::New (_gscene_selected_items, _self));
   proto->Set ("width", v8::FunctionTemplate::New (_gscene_width, _self));
   proto->Set ("height", v8::FunctionTemplate::New (_gscene_height, _self));

   _graphApi.add_function ("createScene", _create_gscene, _self);

}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gbrush_color (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QBrush *brush = self->_to_gbrush (Args.This ());
      if (brush) {

         if (Args.Length () == 1) { brush->setColor (v8_to_qcolor (Args[0])); }
         if (Args.Length () == 3) {

            qreal r, b, g;
            r = v8_to_number (Args[0]);
            g = v8_to_number (Args[1]);
            b = v8_to_number (Args[2]);
            brush->setColor (QColor::fromRgbF (r, g, b));
         }
         result = qcolor_to_v8 (brush->color ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gbrush_style (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QBrush *brush = self->_to_gbrush (Args.This ());
      if (brush) {

         if (Args.Length ()) {

            brush->setStyle ((Qt::BrushStyle)v8_to_uint32 (Args[0]));
         }
         result = v8::Number::New (brush->style ());
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gbrush () {

   v8::HandleScope scope;

   _gBrushTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _gBrushTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gBrushTemp->PrototypeTemplate ();
   proto->Set ("color", v8::FunctionTemplate::New (_gbrush_color, _self));
   proto->Set ("style", v8::FunctionTemplate::New (_gbrush_style, _self));

   _graphApi.add_function ("createBrush", _create_gbrush, _self);

}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpen_brush (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPen *pen = self->_to_gpen (Args.This ());
      if (pen) {

         if (Args.Length ()) {

            QBrush *brush = self->_to_gbrush (Args[0]);
            if (brush) { pen->setBrush (*brush); }
         }
         result = self->create_v8_gbrush (new QBrush (pen->brush ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpen_color (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPen *pen = self->_to_gpen (Args.This ());
      if (pen) {

         if (Args.Length () == 1) { pen->setColor (v8_to_qcolor (Args[0])); }
         if (Args.Length () >= 3) {

            qreal r = v8_to_number (Args[0]);
            qreal g = v8_to_number (Args[1]);
            qreal b = v8_to_number (Args[2]);
            pen->setColor (QColor::fromRgbF (r, g, b));
         }
         result = qcolor_to_v8 (pen->color ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpen_join_style (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPen *pen = self->_to_gpen (Args.This ());
      if (pen) {

         if (Args.Length ()) {

            pen->setJoinStyle ((Qt::PenJoinStyle)v8_to_uint32 (Args[0]));
         }
         result = v8::Number::New (pen->joinStyle ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpen_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPen *pen = self->_to_gpen (Args.This ());
      if (pen) {

         if (Args.Length ()) { pen->setWidthF (v8_to_number (Args[0])); }
         result = v8::Number::New (pen->widthF ());
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gpen () {

   v8::HandleScope scope;

   _gPenTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _gPenTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gPenTemp->PrototypeTemplate ();
   proto->Set ("brush", v8::FunctionTemplate::New (_gpen_brush, _self));
   proto->Set ("color", v8::FunctionTemplate::New (_gpen_color, _self));
   proto->Set ("joinStyle", v8::FunctionTemplate::New (_gpen_join_style, _self));
   proto->Set ("width", v8::FunctionTemplate::New (_gpen_width, _self));

   _graphApi.add_function ("createPen", _create_gpen, _self);

}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_add_ellipse (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () >= 4) {

            qreal x, y, rx, ry;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            rx = v8_to_number (Args[2]);
            ry = v8_to_number (Args[3]);
            path->addEllipse (QPointF(x, y), rx, ry);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_add_path (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length ()) {

            QPainterPath *other = self->_to_gpainter_path (Args[0]);
            if (other) { path->addPath (*other); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_add_rect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () >= 4) {

            qreal x, y, w, h;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);
            path->addRect (x, y, w, h);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_add_rrect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () >= 6) {

            qreal x, y, w, h, xrad, yrad;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);
            xrad = v8_to_number (Args[4]);
            yrad = v8_to_number (Args[5]);
            path->addRoundedRect (x, y, w, h, xrad, yrad);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_angle_at_pct (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length ()) {

            result = v8::Number::New (path->angleAtPercent (v8_to_number (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_arc_move_to (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () >= 5) {

            qreal x, y, w, h, a;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);
            a = v8_to_number (Args[4]);
            path->arcMoveTo (x, y, w, h, a);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_arc_to (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () >= 6) {

            qreal x, y, w, h, startAngle, arcLength;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);
            startAngle = v8_to_number (Args[4]);
            arcLength = v8_to_number (Args[5]);
            path->arcTo (x, y, w, h, startAngle, arcLength);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_brect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) { result = qrectf_to_v8 (path->boundingRect ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_close_subpath (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) { path->closeSubpath (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_connect_path (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length ()) {

            QPainterPath *other = self->_to_gpainter_path (Args.This ());
            if (other) { path->connectPath (*other); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_contains (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () == 1) {

            if (Args[0]->IsObject ()) {

               V8Object obj = v8_to_object (Args[0]);
               if (obj->Has (v8::String::NewSymbol ("x"))) {

                  result = v8::Boolean::New (path->contains (v8_to_qrectf (Args[0])));
               }
            }
            else {

               QPainterPath *other = self->_to_gpainter_path (Args.This ());
               if (other) { result = v8::Boolean::New (path->contains (*other)); }
            }
         }
         else if (Args.Length () == 2) {

            qreal x, y;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);

            result = v8::Boolean::New (path->contains (QPointF (x, y)));
         }
         else if (Args.Length () == 4) {

            qreal x, y, w, h;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);

            result = v8::Boolean::New (path->contains (QRectF (x, y, w, h)));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_cubic (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () == 6) {

            qreal c1x, c1y, c2x, c2y, ex, ey;
            c1x = v8_to_number (Args[0]);
            c1y = v8_to_number (Args[1]);
            c2x = v8_to_number (Args[2]);
            c2y = v8_to_number (Args[3]);
            ex = v8_to_number (Args[4]);
            ey = v8_to_number (Args[5]);

            path->cubicTo (c1x, c1y, c2x, c2y, ex, ey);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_current_position (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         QPointF pos = path->currentPosition ();
         V8Array array = v8::Array::New (2);
         array->Set (v8::Integer::New (0), v8::Number::New (pos.x ()));
         array->Set (v8::Integer::New (1), v8::Number::New (pos.y ()));
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_empty (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) { result = v8::Boolean::New (path->isEmpty ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_intersected (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length ()) {

            QPainterPath *other = self->_to_gpainter_path (Args[0]);
            if (other) {

               result =
                  self->create_v8_gpainter_path (
                     new QPainterPath (path->intersected (*other)));
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_intersects (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () == 1) {

            if (Args[0]->IsObject ()) {

               V8Object obj = v8_to_object (Args[0]);
               if (obj->Has (v8::String::NewSymbol ("x"))) {

                  result = v8::Boolean::New (path->contains (v8_to_qrectf (Args[0])));
               }
            }
            else {

               QPainterPath *other = self->_to_gpainter_path (Args.This ());
               if (other) { result = v8::Boolean::New (path->intersects (*other)); }
            }
         }
         else if (Args.Length () == 4) {

            qreal x, y, w, h;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            w = v8_to_number (Args[2]);
            h = v8_to_number (Args[3]);

            result = v8::Boolean::New (path->intersects (QRectF (x, y, w, h)));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_length (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) { result = v8::Number::New (path->length ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_line_to (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () == 2) {

            qreal x, y;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);

            path->lineTo (x, y);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_move_to (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () == 2) {

            qreal x, y;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);

            path->moveTo (x, y);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_quad_to (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () == 4) {

            qreal x1, y1, x2, y2;
            x1 = v8_to_number (Args[0]);
            y1 = v8_to_number (Args[1]);
            x2 = v8_to_number (Args[2]);
            y2 = v8_to_number (Args[3]);

            path->quadTo (x1, y1, x2, y2);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_simplified (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         result = self->create_v8_gpainter_path (new QPainterPath (path->simplified ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_translate (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () == 2) {

            qreal x, y;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            path->translate (QPointF (x, y));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_translated (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) {

         if (Args.Length () == 2) {

            qreal x, y;
            x = v8_to_number (Args[0]);
            y = v8_to_number (Args[1]);
            result =
               self->create_v8_gpainter_path (
                  new QPainterPath (path->translated (QPointF (x, y))));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpp_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainterPath *path = self->_to_gpainter_path (Args.This ());
      if (path) { result = v8::Number::New (path->elementCount ()); }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gpainter_path () {

   v8::HandleScope scope;

   _gPainterPathTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _gPainterPathTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gPainterPathTemp->PrototypeTemplate ();
   proto->Set ("addEllipse", v8::FunctionTemplate::New (_gpp_add_ellipse, _self));
   proto->Set ("addPath", v8::FunctionTemplate::New (_gpp_add_path, _self));
   proto->Set ("addRect", v8::FunctionTemplate::New (_gpp_add_rect, _self));
   proto->Set ("addRoundedRect", v8::FunctionTemplate::New (_gpp_add_rrect, _self));
   proto->Set ("angleAtPercent", v8::FunctionTemplate::New (_gpp_angle_at_pct, _self));
   proto->Set ("arcMoveTo", v8::FunctionTemplate::New (_gpp_arc_move_to, _self));
   proto->Set ("arcTo", v8::FunctionTemplate::New (_gpp_arc_to, _self));
   proto->Set ("boundingRect", v8::FunctionTemplate::New (_gpp_brect, _self));
   proto->Set ("closeSubpath", v8::FunctionTemplate::New (_gpp_close_subpath, _self));
   proto->Set ("connectPath", v8::FunctionTemplate::New (_gpp_connect_path, _self));
   proto->Set ("contains", v8::FunctionTemplate::New (_gpp_contains, _self));
   proto->Set ("count", v8::FunctionTemplate::New (_gpp_count, _self));
   proto->Set ("cubicTo", v8::FunctionTemplate::New (_gpp_cubic, _self));
   proto->Set (
      "currentPosition",
      v8::FunctionTemplate::New (_gpp_current_position, _self));
   proto->Set ("intersected", v8::FunctionTemplate::New (_gpp_intersected, _self));
   proto->Set ("intersects", v8::FunctionTemplate::New (_gpp_intersects, _self));
   proto->Set ("isEmpty", v8::FunctionTemplate::New (_gpp_empty, _self));
   proto->Set ("length", v8::FunctionTemplate::New (_gpp_length, _self));
   proto->Set ("lineTo", v8::FunctionTemplate::New (_gpp_line_to, _self));
   proto->Set ("moveTo", v8::FunctionTemplate::New (_gpp_move_to, _self));
   proto->Set ("quadTo", v8::FunctionTemplate::New (_gpp_quad_to, _self));
   proto->Set ("simplified", v8::FunctionTemplate::New (_gpp_simplified, _self));
   proto->Set ("translate", v8::FunctionTemplate::New (_gpp_translate, _self));
   proto->Set ("translated", v8::FunctionTemplate::New (_gpp_translated, _self));

   _graphApi.add_function ("createPainterPath", _create_gpainter_path, _self);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpd_height (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPaintDevice *pd = self->_to_gpaint_device (Args.This ());
      if (pd) { result = v8::Number::New (pd->height ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpd_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPaintDevice *pd = self->_to_gpaint_device (Args.This ());
      if (pd) { result = v8::Number::New (pd->width ()); }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gpaint_device () {

   v8::HandleScope scope;

   _gPaintDeviceTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _gPaintDeviceTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gPaintDeviceTemp->PrototypeTemplate ();
   proto->Set ("height", v8::FunctionTemplate::New (_gpd_height, _self));
   proto->Set ("width", v8::FunctionTemplate::New (_gpd_width, _self));
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpixmap_fill (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPixmap *pix = self->_to_gpixmap (Args.This ());
      if (pix) {

         QColor c = Qt::white;
         if (Args.Length ()) { c = v8_to_qcolor (Args[0]); }
         pix->fill (c);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpixmap_load (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPixmap *pix = self->_to_gpixmap (Args.This ());
      if (pix) {

         if (Args.Length ()) {

            QString filename = v8_to_qstring (Args[0]);
            if ((Args.Length () > 1) && Args[1]->IsString ()) {

               pix->load (filename, v8_to_string (Args[1]).get_buffer ());
            }
            else { pix->load (filename); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpixmap_rect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPixmap *pix = self->_to_gpixmap (Args.This ());
      if (pix) { result = qrectf_to_v8 (pix->rect ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpixmap_save (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPixmap *pix = self->_to_gpixmap (Args.This ());
      if (pix) {

         if (Args.Length ()) {

            QString filename = v8_to_qstring (Args[0]);
            if ((Args.Length () > 1) && Args[1]->IsString ()) {

               pix->save (filename, v8_to_string (Args[1]).get_buffer ());
            }
            else { pix->save (filename); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpixmap_scaled (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPixmap *pix = self->_to_gpixmap (Args.This ());
      if (pix) {

         if (Args.Length () > 1) {

            int width = v8_to_uint32 (Args[0]);
            int height = v8_to_uint32 (Args[1]);
            pix->scaled (width, height);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpixmap_to_image (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPixmap *pix = self->_to_gpixmap (Args.This ());
      if (pix) { result = self->create_v8_gimage (new QImage (pix->toImage ())); }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gpixmap () {

   v8::HandleScope scope;

   _gPixmapTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _gPixmapTemp->Inherit (_gPaintDeviceTemp);

   V8ObjectTemplate instance = _gPixmapTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gPixmapTemp->PrototypeTemplate ();
   proto->Set ("fill", v8::FunctionTemplate::New (_gpixmap_fill, _self));
   proto->Set ("load", v8::FunctionTemplate::New (_gpixmap_load, _self));
   proto->Set ("rect", v8::FunctionTemplate::New (_gpixmap_rect, _self));
   proto->Set ("save", v8::FunctionTemplate::New (_gpixmap_save, _self));
   proto->Set ("scaled", v8::FunctionTemplate::New (_gpixmap_scaled, _self));
   proto->Set ("toImage", v8::FunctionTemplate::New (_gpixmap_to_image, _self));

   _graphApi.add_function ("createPixmap", _create_gpixmap, _self);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gimage_format (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QImage *image = self->_to_gimage (Args.This ());
      if (image) {

         if (Args.Length ()) {

            image->convertToFormat ((QImage::Format)v8_to_uint32 (Args[0]));
         }
         result = v8::Number::New ((QImage::Format) image->format ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gimage_rect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QImage *image = self->_to_gimage (Args.This ());
      if (image) { result = qrectf_to_v8 (image->rect ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gimage_load (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QImage *image = self->_to_gimage (Args.This ());
      if (image) {

         if (Args.Length ()) {

            QString filename = v8_to_qstring (Args[0]);
            if ((Args.Length () > 1) && Args[1]->IsString ()) {

               image->load (filename, v8_to_string (Args[1]).get_buffer ());
            }
            else { image->load (filename); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gimage_save (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QImage *image = self->_to_gimage (Args.This ());
      if (image) {

         if (Args.Length ()) {

            QString filename = v8_to_qstring (Args[0]);
            if ((Args.Length () > 1) && Args[1]->IsString ()) {

               image->save (filename, v8_to_string (Args[1]).get_buffer ());
            }
            else { image->save (filename); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gimage_scaled (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QImage *image = self->_to_gimage (Args.This ());
      if (image) {

         if (Args.Length () > 1) {

            int width, height;
            width = v8_to_uint32 (Args[0]);
            height = v8_to_uint32 (Args[1]);
            image->scaled (width, height);
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gimage () {

   v8::HandleScope scope;

   _gImageTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _gImageTemp->Inherit (_gPaintDeviceTemp);

   V8ObjectTemplate instance = _gImageTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gImageTemp->PrototypeTemplate ();
   proto->Set ("format", v8::FunctionTemplate::New (_gimage_format, _self));
   proto->Set ("rect", v8::FunctionTemplate::New (_gimage_rect, _self));
   proto->Set ("save", v8::FunctionTemplate::New (_gimage_save, _self));
   proto->Set ("load", v8::FunctionTemplate::New (_gimage_load, _self));
   proto->Set ("scaled", v8::FunctionTemplate::New (_gimage_scaled, _self));

   _graphApi.add_function ("createImage", _create_gimage, _self);
   _graphApi.add_constant ("FormatInvalid", (UInt32)QImage::Format_Invalid);
   _graphApi.add_constant ("FormatMono", (UInt32)QImage::Format_Mono);
   _graphApi.add_constant ("FormatMonoLSB", (UInt32)QImage::Format_MonoLSB);
   _graphApi.add_constant ("FormatIndexed8", (UInt32)QImage::Format_Indexed8);
   _graphApi.add_constant ("FormatRGB32", (UInt32)QImage::Format_RGB32);
   _graphApi.add_constant ("FormatARGB32", (UInt32)QImage::Format_ARGB32);
   _graphApi.add_constant (
      "FormatARGB32Premultiplied",
      (UInt32)QImage::Format_ARGB32_Premultiplied);
   _graphApi.add_constant ("FormatRGB16", (UInt32)QImage::Format_RGB16);
   _graphApi.add_constant (
      "FormatARGB8565Premultiplied",
      (UInt32)QImage::Format_ARGB8565_Premultiplied);
   _graphApi.add_constant ("FormatRGB666", (UInt32)QImage::Format_RGB666);
   _graphApi.add_constant (
      "FormatARGB6666Premultiplied",
      (UInt32)QImage::Format_ARGB6666_Premultiplied);
   _graphApi.add_constant ("FormatRGB555", (UInt32)QImage::Format_RGB555);
   _graphApi.add_constant (
      "FormatARGB8555Premultiplied",
      (UInt32)QImage::Format_ARGB8555_Premultiplied);
   _graphApi.add_constant ("FormatRGB888", (UInt32)QImage::Format_RGB888);
   _graphApi.add_constant ("FormatRGB444", (UInt32)QImage::Format_RGB444);
   _graphApi.add_constant (
      "FormatARGB4444",
      (UInt32)QImage::Format_ARGB4444_Premultiplied);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_gpainter_end (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPainter *painter = self->_to_gpainter (Args.This ());
      if (painter) { painter->end (); }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_gpainter () {

   v8::HandleScope scope;

   _gPainterTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _gPainterTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gPainterTemp->PrototypeTemplate ();
   proto->Set ("end", v8::FunctionTemplate::New (_gpainter_end, _self));

   _graphApi.add_function ("createPainter", _create_gpainter, _self);
}
