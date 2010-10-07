#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QLabel>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_label_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());

      if (widget) {

         QLabel *label = qobject_cast<QLabel *>(widget);

         if (label) {

            if (Args.Length ()) {

               label->setText (v8_to_string (Args[0]).get_buffer ());
            }
            else {

               result = v8::String::New (qPrintable(label->text ()));
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_label_word_wrap (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());

      if (widget) {

         QLabel *label = qobject_cast<QLabel *>(widget);

         if (label) {

            if (Args.Length ()) {

               label->setWordWrap (v8_to_boolean (Args[0]));
            }
            else {

               result = v8::Boolean::New (label->wordWrap ());
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_label_clear (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());

      if (widget) {

         QLabel *label = qobject_cast<QLabel *>(widget);

         if (label) {

            label->clear ();
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_label () {

   v8::HandleScope scope;

   _labelTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _labelTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _labelTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _labelTemp->PrototypeTemplate ();
   proto->Set ("text", v8::FunctionTemplate::New (_label_text, _self));
   proto->Set ("wordWrap", v8::FunctionTemplate::New (_label_word_wrap, _self));
   proto->Set ("clear", v8::FunctionTemplate::New (_label_clear, _self));
}
