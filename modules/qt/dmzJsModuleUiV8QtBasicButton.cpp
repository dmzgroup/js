#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QAbstractButton>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_button_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      String param = v8_to_string (Args[0]);

      if (widget && param) {

         QAbstractButton *button = qobject_cast<QAbstractButton *>(widget);
         
         if (button) {

            button->setText (param.get_buffer ());
         }
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
}
