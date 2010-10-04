#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtTabWidget.h"
#include <QtGui/QTabWidget>

#include <QtCore/QDebug>


namespace {

   static const dmz::String CurrentChanged ("currentChanged");
};


dmz::V8QtTabWidget::V8QtTabWidget (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtTabWidget::~V8QtTabWidget () {;}


dmz::Boolean
dmz::V8QtTabWidget::bind (
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
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtTabWidget::on_currentChanged (int val) {

   _do_callback (CurrentChanged, val);
}
