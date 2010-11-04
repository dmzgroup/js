#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"
#include <QtGui/QAbstractButton>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_button_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAbstractButton *button = self->v8_to_qobject<QAbstractButton> (Args.This ());;
      if (button) {

         if (Args.Length ()) {

            button->setText (v8_to_qstring (Args[0]));
         }

         result = qstring_to_v8 (button->text ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_button_is_checked (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAbstractButton *button = self->v8_to_qobject<QAbstractButton> (Args.This ());;
      if (button) {

         result = v8::Boolean::New (button->isChecked ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_button_set_checked (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAbstractButton *button = self->v8_to_qobject<QAbstractButton> (Args.This ());;
      if (button) {

         if (Args.Length ()) {

            button->setChecked (v8_to_boolean (Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_button_click (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAbstractButton *button = self->v8_to_qobject<QAbstractButton> (Args.This ());;
      if (button) {

         button->click ();
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_button () {

   v8::HandleScope scope;

   _buttonTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _buttonTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _buttonTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _buttonTemp->PrototypeTemplate ();
   proto->Set ("text", v8::FunctionTemplate::New (_button_text, _self));
   proto->Set ("isChecked", v8::FunctionTemplate::New (_button_is_checked, _self));
   proto->Set ("setChecked", v8::FunctionTemplate::New (_button_set_checked, _self));
   proto->Set ("click", v8::FunctionTemplate::New (_button_click, _self));
}
