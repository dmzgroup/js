#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzV8QtUtil.h>
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QDateTimeEdit>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_dt_maximum (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QDateTimeEdit *dt = self->v8_to_qobject<QDateTimeEdit>(Args.This ());
      if (dt) {

         if (Args.Length () && Args[0]->IsDate ()) {

            dt->setMaximumDateTime (v8_to_qdatetime (Args[0]));
         }
         result = qdatetime_to_v8 (dt->maximumDateTime ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_dt_minimum (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QDateTimeEdit *dt = self->v8_to_qobject<QDateTimeEdit>(Args.This ());
      if (dt) {

         if (Args.Length () && Args[0]->IsDate ()) {

            dt->setMinimumDateTime (v8_to_qdatetime (Args[0]));
         }
         result = qdatetime_to_v8 (dt->minimumDateTime ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_dt_datetime (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QDateTimeEdit *dt = self->v8_to_qobject<QDateTimeEdit>(Args.This ());
      if (dt) {

         if (Args.Length () && Args[0]->IsDate ()) {

            dt->setDateTime (v8_to_qdatetime (Args[0]));
         }
         result = qdatetime_to_v8 (dt->dateTime ());
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_dt () {

   v8::HandleScope scope;

   _dteTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _dteTemp->Inherit (_spinBoxTemp);

   V8ObjectTemplate instance = _dteTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _dteTemp->PrototypeTemplate ();
   proto->Set ("maximum", v8::FunctionTemplate::New (_dt_maximum, _self));
   proto->Set ("minimum", v8::FunctionTemplate::New (_dt_minimum, _self));
   proto->Set ("dateTime", v8::FunctionTemplate::New (_dt_datetime, _self));
}
