#include "dmzV8QtWidget.h"
#include <QtGui/QWidget>

using namespace dmz;

namespace {

   void
   local_v8_qt_widget_delete (v8::Persistent<v8::Value> object, void *param) {

      if (param) {

         V8QtWidget *ptr = (V8QtWidget *)param;
         ptr->clean_up ();
         delete ptr;
         ptr = 0;
      }

      object.Dispose (); object.Clear ();
   }
};


dmz::V8QtWidget::V8QtWidget (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state),
      _widget (widget) {

   setObjectName ("V8QtWidget");
}


dmz::V8QtWidget::~V8QtWidget () {

   if (_widget) {

      if (!_widget->parentWidget ()) {

         if (_deleteObject) { delete _widget; }
      }
   }

   _widget = 0;
}


QWidget *
dmz::V8QtWidget::get_qwidget () const {

   return _widget;
}


dmz::Boolean
dmz::V8QtWidget::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   return False;
}


void
dmz::V8QtWidget::clean_up () {

   if (_state && _state->ui) {

      _state->ui->v8_qt_widget_destroyed (this);
   }
}


void
dmz::V8QtWidget::_make_weak (const Boolean Value) {

   if (Value) { self.MakeWeak ((void *)this, local_v8_qt_widget_delete); }
   else { self.ClearWeak (); }
}
