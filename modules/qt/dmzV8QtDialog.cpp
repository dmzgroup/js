#include <dmzJsModuleV8.h>
#include "dmzV8QtDialog.h"
#include <QtGui/QDialog>

namespace {

   static const dmz::String LocalSignalFinished ("finished");
};


dmz::V8QtDialog::V8QtDialog (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtDialog::~V8QtDialog () {;}


dmz::Boolean
dmz::V8QtDialog::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == LocalSignalFinished) {

         connect (
            _widget,
            SIGNAL (finished (int)),
            SLOT (on_finished (int)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtDialog::open (
      const V8Object &Self,
      const V8Function &Func) {

   QDialog *dialog = qobject_cast<QDialog *>(_widget);

   if (dialog) {

      if (bind (LocalSignalFinished, Self, Func)) {

         dialog->open ();
      }
   }
}


void
dmz::V8QtDialog::on_finished (int value) {

   _do_callback (LocalSignalFinished, value);
}
