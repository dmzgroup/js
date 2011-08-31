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

      V8QtDialog *jsDialog = self->_to_v8_qt_dialog (Args.This ());
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

            if (os) { os->list.append (jsDialog); }

            jsDialog->open (src, func);
         }

         result = func;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_dialog_accept (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QDialog *dialog = self->v8_to_qobject<QDialog>(Args.This ());
      if (dialog) { dialog->accept (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_dialog_reject (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QDialog *dialog = self->v8_to_qobject<QDialog>(Args.This ());
      if (dialog) { dialog->reject (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_dialog_set_windows_hint (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QDialog *dialog = self->v8_to_qobject<QDialog>(Args.This ());
      if (dialog) {

         Qt::WindowFlags flags = dialog->windowFlags ();
         flags |= Qt::FramelessWindowHint;
         dialog->setWindowFlags (flags);

         QPoint pos = dialog->pos ();
         if (pos.x () < 0) { pos.setX (0); }
         if (pos.y () < 0) { pos.setY (0); }
         dialog->move (pos);
         dialog->show ();
      }
   }

   return scope.Close (result);
}


dmz::V8QtDialog *
dmz::JsModuleUiV8QtBasic::_to_v8_qt_dialog (V8Value value) {

   V8QtDialog *result (0);
   V8QtObject *object = _to_v8_qt_object (value);
   if (object) { result = qobject_cast<V8QtDialog *>(object); }
   return result;
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
   proto->Set ("accept", v8::FunctionTemplate::New (_dialog_accept, _self));
   proto->Set ("reject", v8::FunctionTemplate::New (_dialog_reject, _self));
   proto->Set ("setWindowsHint", v8::FunctionTemplate::New (_dialog_set_windows_hint, _self));
}
