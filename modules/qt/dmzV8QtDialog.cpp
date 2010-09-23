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

               const int Argc (2);
               V8Value argv[Argc];
               argv[0] = v8::Number::New (value);
               argv[1] = cs->self;

               v8::TryCatch tc;

               cs->func->Call (cs->self, Argc, argv);

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
