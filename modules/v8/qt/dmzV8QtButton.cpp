#include "dmzV8QtButton.h"
#include <QtGui/QWidget>


dmz::V8QtButton::V8QtButton (QWidget *widget, QObject *parent) :
      V8QtObject (widget, parent) {;}


dmz::V8QtButton::~V8QtButton () {;}


dmz::Boolean
dmz::V8QtButton::bind (QWidget *sender, const String &Signal) {

   Boolean results (False);

   if (sender) {
      
      if (Signal == "clicked") {
   
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

   v8::HandleScope scope;
   
   foreach (CallbackStruct *cbs, _cbList) {
      
      if (!cbs->func.IsEmpty () && !cbs->self.IsEmpty ()) {

         // V8Object localSelf = v8::Local<v8::Object>::New (cbs->self);
         // V8Function localFunc = v8::Local<v8::Function>::New (cbs->func);

         V8Value argv[] = { cbs->self };
         // V8Value argv[] = { localSelf };
         
         v8::TryCatch tc;
         
         cbs->func->Call (cbs->self, 1, argv);
         // localFunc->Call (localSelf, 1, argv);

         if (tc.HasCaught ()) {

            // if (_core) { _core->handle_v8_exception (tc); }
         }
      }
   }
}   

