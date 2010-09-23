#include <dmzJsModuleV8.h>
#include "dmzV8QtButton.h"
#include <QtGui/QWidget>

namespace {
   
   static const dmz::String LocalSignalClicked ("clicked");
   static const dmz::String ToggledSignal ("toggled");
};


dmz::V8QtButton::V8QtButton (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtButton::~V8QtButton () {;}


dmz::Boolean
dmz::V8QtButton::bind (QWidget *sender, const String &Signal) {

   Boolean results (False);

   if (sender) {
      
      if (Signal == LocalSignalClicked) {
   
         connect (
            sender,
            SIGNAL (clicked ()),
            SLOT (on_clicked ()),
            Qt::UniqueConnection);
            
         results = True;
      }

      else if (Signal == ToggledSignal) {

         connect (
            sender,
            SIGNAL (toggled (bool)),
            SLOT (on_toggle (bool)),
            Qt::UniqueConnection);

         results = True;
      }
   }
   
   if (!results) { results = V8QtObject::bind (sender, Signal); }
   
   return results;
}


void
dmz::V8QtButton::on_clicked () {

   if (_state && _state->core) {
      
      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      CallbackTable *ct = _cbTable.lookup (LocalSignalClicked);
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


void
dmz::V8QtButton::on_toggle (bool checked) {

   if (_state && _state->core && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      CallbackTable *ct = _cbTable.lookup (ToggledSignal);
      if (ct) {

         HashTableHandleIterator it;
         CallbackStruct *cs (0);

         while (ct->table.get_next (it, cs)) {

            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {

               const Handle Observer = cs->Observer;

               const int Argc (2);
               V8Value argv[Argc];
               argv[0] = v8::Boolean::New (checked);
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
