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
      V8QtObject (Self, widget, state) {;}


dmz::V8QtDialog::~V8QtDialog () {;}


dmz::Boolean
dmz::V8QtDialog::bind (QWidget *sender, const String &Signal) {

   Boolean results (False);

   if (sender) {

      if (Signal == LocalSignalFinished) {

         connect (
            sender,
            SIGNAL (finished (int)),
            SLOT (on_finished (int)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (!results) { results = V8QtObject::bind (sender, Signal); }

   return results;
}


void
dmz::V8QtDialog::open (
      const V8Object &Self,
      const V8Function &Func) {

   QDialog *dialog = qobject_cast<QDialog *>(_widget);

   if (dialog) {

      if (bind (dialog, LocalSignalFinished)) {

         register_callback (LocalSignalFinished, Self, Func);

         dialog->open ();
      }
   }
}


void
dmz::V8QtDialog::on_finished (int value) {

   if (_state && _state->core) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      CallbackTable *ct = _cbTable.lookup (LocalSignalFinished);
      if (ct) {

         HashTableHandleIterator it;
         CallbackStruct *cs (0);

         while (ct->table.get_next (it, cs)) {

            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {

               const Handle Observer = cs->Observer;

               V8Value argv[] = { cs->self };

               v8::TryCatch tc;

               cs->func->Call (cs->self, 1, argv);

               if (tc.HasCaught ()) {

                  _state->core->handle_v8_exception (Observer, tc);

                  cs = ct->table.remove (Observer);

                  if (cs) { delete cs; cs = 0; }
               }
            }
         }
      }
   }
}
