#include "dmzJsModuleUiV8QtBasic.h"
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
      
self->_log.warn << "widget->close ()" << endl;
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

   if (self && Args[0]->IsObject () && Args[2]->IsFunction ()) {

      V8QtObject *jsObject = self->_to_js_qt_object (Args.This ());
      if (jsObject) {
         
         QWidget *qtWidget = jsObject->get_qt_widget ();

         V8Object src = V8Object::Cast (Args[0]);
         const String Signal = v8_to_string (Args[1]).to_lower ();
         V8Function func = v8_to_function (Args[2]);

         if (qtWidget) {

            if (jsObject->bind (qtWidget, Signal)) {
               
               jsObject->add_callback (src, func);
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
