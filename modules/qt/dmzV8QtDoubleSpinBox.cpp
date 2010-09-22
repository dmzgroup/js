#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtDoubleSpinBox.h"
#include <QtGui/QDoubleSpinBox>

#include <QtCore/QDebug>


namespace {

   static const dmz::String ValueChanged ("valueChanged");
};


dmz::V8QtDoubleSpinBox::V8QtDoubleSpinBox (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtDoubleSpinBox::~V8QtDoubleSpinBox () {;}


dmz::Boolean
dmz::V8QtDoubleSpinBox::bind (QWidget *sender, const String &Signal) {

   Boolean results (False);

   if (sender) {

      if (Signal == ValueChanged) {

         connect (
            sender,
            SIGNAL (valueChanged (double)),
            this,
            SLOT (on_valueChanged (double)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (!results) { results = V8QtObject::bind (sender, Signal); }

   return results;
}

void
dmz::V8QtDoubleSpinBox::on_valueChanged (double val) {

   if (_state && _state->core && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      CallbackTable *ct = _cbTable.lookup (ValueChanged);
      if (ct) {

         HashTableHandleIterator it;
         CallbackStruct *cs (0);

         while (ct->table.get_next (it, cs)) {

            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {

               const Handle Observer = cs->Observer;

               const int Argc (2);
               V8Value argv[Argc];
               argv[0] = v8::Number::New (val);
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

