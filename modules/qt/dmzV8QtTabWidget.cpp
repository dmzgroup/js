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

   if (_state && _state->core && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      CallbackTable *ct = _cbTable.lookup (CurrentChanged);
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
