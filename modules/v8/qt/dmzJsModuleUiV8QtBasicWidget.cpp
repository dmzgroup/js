#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtObject.h"
#include <QtGui/QWidget>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_close (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) {
      
         widget->close ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_enabled (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) {

         if (Args.Length ()) {
            
            const Boolean Value = v8_to_boolean (Args[0]);
            widget->setEnabled (Value);
         }
         else {
            
            result = v8::Boolean::New (widget->isEnabled ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_hide (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) { widget->hide (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_lookup (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      String param = v8_to_string (Args[0]);

      if (widget && param) {

         QWidget *child = widget->findChild<QWidget *>(param.get_buffer ());

         if (child) {

            result = self->create_v8_widget (child);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   // if (Args.Length () >= 3) {
   //    
   // }
   
   if (self && v8_is_object (Args[0]) && v8_is_function (Args[2])) {

      V8QtObject *jsObject = self->_to_js_qt_object (Args.This ());
      if (jsObject) {
         
         QWidget *qtWidget = jsObject->get_qt_widget ();

         V8Object src = v8_to_object (Args[0]);
         const String Signal = v8_to_string (Args[1]);
         V8Function func = v8_to_function (Args[2]);

         const Handle Obs =
            self->_state.core ? self->_state.core->get_instance_handle (src) : 0;

         if (qtWidget && Obs) {

            // connect the signal from qtWidget to the desired slot of jsObject
            if (jsObject->bind (qtWidget, Signal)) {
               
               jsObject->register_callback (Signal, src, func);

               ObsStruct *os = self->_obsTable.lookup (Obs);
               if (!os) {
                  
                  os = new ObsStruct (Obs);
                  if (!self->_obsTable.store (Obs, os)) { delete os; os = 0; }
               }
               
               if (os) { os->list.append (jsObject); }
            }
         }
         
         result = func;
      }
   }

   return scope.Close (result);
}


#if 0
dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_property (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) {

         // if (Args.Lengt () > 2) {
         //    
         //    const String Value = v8_to_string (Args[1]);
         // }
         // else {
         //    
         //    String Value;
         // }
      }
   }

   return scope.Close (result);
}
#endif


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_show (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) { widget->show (); }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_widget () {

   v8::HandleScope scope;

   _widgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _widgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _widgetTemp->PrototypeTemplate ();
   proto->Set ("close", v8::FunctionTemplate::New (_widget_close, _self));
   proto->Set ("enabled", v8::FunctionTemplate::New (_widget_enabled, _self));
   proto->Set ("lookup", v8::FunctionTemplate::New (_widget_lookup, _self));
   proto->Set ("show", v8::FunctionTemplate::New (_widget_show, _self));
   proto->Set ("hide", v8::FunctionTemplate::New (_widget_hide, _self));
   proto->Set ("observe", v8::FunctionTemplate::New (_widget_observe, _self));
}

