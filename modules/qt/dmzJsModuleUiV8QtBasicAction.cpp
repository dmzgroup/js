#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QAbstractButton>

/*
dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_action (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      // QWidget *widget = self->_to_qt_widget (Args.This ());
      // String param = v8_to_string (Args[0]);
      //
      // if (widget && param) {
      //
      //    QAbstractButton *button = qobject_cast<QAbstractButton *>(widget);
      //
      //    if (button) {
      //
      //       button->setText (param.get_buffer ());
      //    }
      // }
   }

   return scope.Close (result);
}
*/


void
dmz::JsModuleUiV8QtBasic::_init_action () {

   v8::HandleScope scope;

   _actionTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _actionTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _actionTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _actionTemp->PrototypeTemplate ();
   // proto->Set ("iconText", v8::FunctionTemplate::New (_action_icon_text, _self));
   // proto->Set ("text", v8::FunctionTemplate::New (_action_text, _self));
   // proto->Set ("statusText", v8::FunctionTemplate::New (_action_status_text, _self));
   // proto->Set ("toolTip", v8::FunctionTemplate::New (_action_tool_tip, _self));
   // proto->Set ("whatsThis", v8::FunctionTemplate::New (_action_whats_this, _self));
}
