#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtStackedWidget.h"
#include <QtGui/QStackedWidget>

#include <QtCore/QDebug>


namespace {

   static const dmz::String CurrentChanged ("currentChanged");
   static const dmz::String WidgetRemoved ("widgetRemoved");
};


dmz::V8QtStackedWidget::V8QtStackedWidget (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtStackedWidget::~V8QtStackedWidget () {;}


dmz::Boolean
dmz::V8QtStackedWidget::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {


      if (Signal == CurrentChanged) {

         connect (
            _widget,
            SIGNAL (currentChanged (int)),
            SLOT (on_currentChanged (int)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == WidgetRemoved) {

         connect (
            _widget,
            SIGNAL (widgetRemoved (int)),
            SLOT (on_widgetRemoved (int)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtStackedWidget::on_currentChanged (int val) {

   _do_callback (CurrentChanged, val);
}


void
dmz::V8QtStackedWidget::on_widgetRemoved (int val) {

   _do_callback (WidgetRemoved, val);
}
