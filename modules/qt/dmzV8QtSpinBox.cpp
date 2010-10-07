#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtSpinBox.h"
#include <QtGui/QSpinBox>


namespace {

   static const dmz::String ValueChanged ("valueChanged");
};


dmz::V8QtSpinBox::V8QtSpinBox (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtSpinBox::~V8QtSpinBox () {;}


dmz::Boolean
dmz::V8QtSpinBox::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == ValueChanged) {

         if (_widget->inherits ("QSpinBox")) {

            connect (
               _widget,
               SIGNAL (valueChanged (int)),
               SLOT (on_valueChanged (int)),
               Qt::UniqueConnection);
         }
         else {

            connect (
               _widget,
               SIGNAL (valueChanged (double)),
               SLOT (on_valueChanged (double)),
               Qt::UniqueConnection);
         }

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtSpinBox::on_valueChanged (int val) {

   _do_callback (ValueChanged, val);
}


void
dmz::V8QtSpinBox::on_valueChanged (double val) {

   _do_callback (ValueChanged, val);
}
