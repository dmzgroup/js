#include <dmzJsModuleV8.h>
#include "dmzV8QtButton.h"
#include <QtGui/QWidget>

namespace {
   
   static const dmz::String LocalSignalClicked ("clicked");
};


dmz::V8QtButton::V8QtButton (QWidget *widget, JsModuleUiV8QtBasic::State *state) :
      V8QtObject (widget, state) {;}


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
   }
   
   if (!results) { results = V8QtObject::bind (sender, Signal); }
   
   return results;
}


void
dmz::V8QtButton::on_clicked () {

   if (_state) {
      
      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      ObsStruct *os = _obsTable.lookup (LocalSignalClicked);
      if (os) {
         
         CallbackStruct *cs (os->list);
         while (cs) {
            
            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {

               V8Value argv[] = { cs->self };

               v8::TryCatch tc;

               cs->func->Call (cs->self, 1, argv);

               if (tc.HasCaught ()) {

                  if (_state->core) { _state->core->handle_v8_exception (tc); }
               }
            }

            cs = cs->next;
         }
      }
   }
}
