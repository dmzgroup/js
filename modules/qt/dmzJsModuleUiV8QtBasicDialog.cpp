#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzV8QtDialog.h>
#include <QtGui/QDialog>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_dialog_open (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self && v8_is_object (Args[0]) && v8_is_function (Args[1])) {

      V8QtObject *jsObject = self->_to_js_qt_object (Args.This ());
      if (jsObject) {

         V8QtDialog *jsDialog = qobject_cast<V8QtDialog *>(jsObject);

         if (jsDialog) {

            V8Object src = v8_to_object (Args[0]);
            V8Function func = v8_to_function (Args[1]);

            const Handle Obs =
               self->_state.core ? self->_state.core->get_instance_handle (src) : 0;

            if (Obs) {

               ObsStruct *os = self->_obsTable.lookup (Obs);
               if (!os) {

                  os = new ObsStruct (Obs);
                  if (!self->_obsTable.store (Obs, os)) { delete os; os = 0; }
               }

               if (os) { os->list.append (jsObject); }

               jsDialog->open (src, func);
            }

            result = func;
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_dialog () {

   v8::HandleScope scope;

   _dialogTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _dialogTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _dialogTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _dialogTemp->PrototypeTemplate ();
   proto->Set ("open", v8::FunctionTemplate::New (_dialog_open, _self));
}
