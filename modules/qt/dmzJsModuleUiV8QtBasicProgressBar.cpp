#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QProgressBar>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_progress_bar_maximum (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());

      if (widget) {

         QProgressBar *pbar = qobject_cast<QProgressBar *>(widget);

         if (pbar) {

            if (Args.Length ()) {

               pbar->setMaximum (v8_to_number (Args[0]));
            }
            else {

               result = v8::Number::New (pbar->maximum ());
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_progress_bar_minimum (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());

      if (widget) {

         QProgressBar *pbar = qobject_cast<QProgressBar *>(widget);

         if (pbar) {

            if (Args.Length ()) {

               pbar->setMinimum (v8_to_number (Args[0]));
            }
            else {

               result = v8::Number::New (pbar->minimum ());
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_progress_bar_value (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());

      if (widget) {

         QProgressBar *pbar = qobject_cast<QProgressBar *>(widget);

         if (pbar) {

            if (Args.Length ()) {

               pbar->setValue (v8_to_number (Args[0]));
            }
            else {

               result = v8::Number::New (pbar->value ());
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_progress_bar_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());

      if (widget) {

         QProgressBar *pbar = qobject_cast<QProgressBar *>(widget);

         if (pbar) {

            result = v8::String::New (qPrintable (pbar->text ()));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_progress_bar_reset (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());

      if (widget) {

         QProgressBar *pbar = qobject_cast<QProgressBar *>(widget);

         if (pbar) {

            pbar->reset ();
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_progressBar () {

   v8::HandleScope scope;

   _progressBarTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _progressBarTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _progressBarTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _progressBarTemp->PrototypeTemplate ();
   proto->Set ("value", v8::FunctionTemplate::New (_progress_bar_value, _self));
   proto->Set ("maximum", v8::FunctionTemplate::New (_progress_bar_maximum, _self));
   proto->Set ("minimum", v8::FunctionTemplate::New (_progress_bar_minimum, _self));
   proto->Set ("text", v8::FunctionTemplate::New (_progress_bar_text, _self));
   proto->Set ("reset", v8::FunctionTemplate::New (_progress_bar_reset, _self));
}
