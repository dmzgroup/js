#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QLCDNumber>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_lcd_number_value (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QLCDNumber *lcd = self->v8_to_qobject<QLCDNumber>(Args.This ());
      if (lcd) {

         if (Args.Length ()) {

            lcd->display (v8_to_number (Args[0]));
         }
         else {

            result = v8::Number::New (lcd->value ());
         }
      }
   }

   return scope.Close (result);
}

dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_lcd (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      UInt32 numDigits = 0;
      if (Args.Length ()) {

         if (Args.Length () > 0) {

            numDigits = v8_to_uint32 (Args[0]);
            parent = self->_to_qwidget (Args[1]);
         }
         else { parent = self->_to_qwidget (Args[0]); }
      }

      QLCDNumber *widget = 0;
      if (numDigits) { widget = new QLCDNumber (numDigits, parent); }
      else { widget = new QLCDNumber (parent); }
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}

void
dmz::JsModuleUiV8QtBasic::_init_lcdNumber () {

   v8::HandleScope scope;

   _lcdNumberTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _lcdNumberTemp->Inherit (_frameTemp);

   V8ObjectTemplate instance = _lcdNumberTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _lcdNumberTemp->PrototypeTemplate ();
   proto->Set ("value", v8::FunctionTemplate::New (_lcd_number_value, _self));

   _lcdApi.add_function ("create", _create_lcd, _self);
}
