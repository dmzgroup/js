#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QLCDNumber>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_lcd_number_value (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QLCDNumber *lcd = qobject_cast<QLCDNumber *>(widget);

         if (lcd) {

            if (Args.Length ()) {

               lcd->display (v8_to_number (Args[0]));
            }
            else {

               result = v8::Number::New (lcd->value ());
            }
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_lcdNumber () {

   v8::HandleScope scope;

   _lcdNumberTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _lcdNumberTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _lcdNumberTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _lcdNumberTemp->PrototypeTemplate ();
   proto->Set ("value", v8::FunctionTemplate::New (_lcd_number_value, _self));
}
