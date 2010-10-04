#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtProgressBar.h"
#include <QtGui/QProgressBar>
#include <QtCore/QDebug>


namespace {

   static const dmz::String ValueChanged ("valueChanged");
};


dmz::V8QtProgressBar::V8QtProgressBar (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtProgressBar::~V8QtProgressBar () {;}


dmz::Boolean
dmz::V8QtProgressBar::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == ValueChanged) {

         connect (
            _widget,
            SIGNAL (valueChanged (int)),
            SLOT (on_valueChanged (int)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtProgressBar::on_valueChanged (int val) {

   _do_callback (ValueChanged, val);
}
