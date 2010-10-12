#include "dmzV8QtWidget.h"
#include <QtGui/QWidget>


dmz::V8QtWidget::V8QtWidget (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state),
      _widget (widget) {;}


dmz::V8QtWidget::~V8QtWidget () {

   if (_widget) {

      if (!_widget->parentWidget () && _deleteObject) { delete _widget; }
      _widget = 0;
      _object = 0;
   }
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
