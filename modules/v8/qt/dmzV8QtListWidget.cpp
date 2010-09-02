#include "dmzV8QtListWidget.h"
// #include <QtGui/QtGui>


dmz::V8QtListWidget::V8QtListWidget (QWidget *widget, JsModuleUiV8QtBasic::State *state) :
      V8QtObject (widget, state) {;}


dmz::V8QtListWidget::~V8QtListWidget () {;}


dmz::Boolean
dmz::V8QtListWidget::bind (QWidget *sender, const String &Signal) {

   Boolean results (False);

   if (sender) {
      
      // if (Signal == "clicked") {
      //    
      //    connect (
      //       sender,
      //       SIGNAL (clicked ()),
      //       SLOT (on_clicked ()),
      //       Qt::UniqueConnection);
      //       
      //    results = True;
      // }
   }
   
   if (!results) { results = V8QtObject::bind (sender, Signal); }
   
   return results;
}


/*
void
dmz::V8QtListWidget::on_clicked () {

   if (_state) {
      
      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      foreach (CallbackStruct *cbs, _cbList) {

         if (!(cbs->func.IsEmpty ()) && !(cbs->self.IsEmpty ())) {

            V8Value argv[] = { cbs->self };

            v8::TryCatch tc;

            cbs->func->Call (cbs->self, 1, argv);

            if (tc.HasCaught ()) {

               if (_state->core) { _state->core->handle_v8_exception (tc); }
            }
         }
      }
   }
}
*/